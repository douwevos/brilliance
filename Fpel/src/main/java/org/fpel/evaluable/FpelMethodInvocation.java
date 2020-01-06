package org.fpel.evaluable;

import java.lang.reflect.Method;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.stream.Stream;

import org.fpel.FpelContext;
import org.fpel.FpelEvalContext;
import org.fpel.FpelEvaluable;
import org.fpel.FpelNode;
import org.fpel.FpelToken;

import lombok.AllArgsConstructor;

public class FpelMethodInvocation extends FpelEvaluable {

    FpelEvaluable leftHand;
    FpelToken methodNameToken;
    String methodName;
    List<FpelEvaluable> arguments;

    public volatile MethodInvokerCache invokerCache;

    public FpelMethodInvocation(FpelEvaluable leftHand, FpelToken methodNameToken, List<FpelEvaluable> arguments) {
    	super(createArray(leftHand, methodNameToken, arguments));
        this.leftHand = leftHand;
        this.methodNameToken = methodNameToken;
        this.methodName = (String) methodNameToken.getValue();
        this.arguments = arguments;
    }

    private static FpelNode[] createArray(FpelEvaluable leftHand, FpelToken methodNameToken,
			List<FpelEvaluable> arguments) {
    	List<FpelNode> nodes = new ArrayList<FpelNode>();
    	nodes.add(leftHand);
    	nodes.add(methodNameToken);
    	if (arguments != null) {
    		nodes.addAll(arguments);
    	}
		return nodes.toArray(new FpelNode[nodes.size()]);
	}
    
    @Override
    public FpelNode getLHS() {
    	return leftHand;
    }

    @Override
    public String name() {
    	return methodName;
    }
    
    @Override
    public void eval(FpelContext context, FpelEvalContext evalContext) {
        Object left = null;
        if (leftHand != null) {
            leftHand.eval(context, evalContext);
            left = evalContext.peek();
            if (left == null) {
                return;
            }
        }

        Object[] argValues = new Object[arguments.size()];
        int argIdx = 0;
        for (FpelEvaluable arg : arguments) {
            arg.eval(context, evalContext);
            Object argVal = evalContext.peek();
            argValues[argIdx] = argVal;
            evalContext.pop(1);
            argIdx++;
        }

        MethodInvokerCache methodInvokerCacheLocal = invokerCache;

        if (methodInvokerCacheLocal == null) {

            List<Method> methodsList = null;

            Class<?> leftClass = null;

            if (left == null) {
                methodsList = new ArrayList<>();
                Method method = context.getFunction(methodName);
                if (method != null) {
                    methodsList.add(method);
                }
            }
            else {
                leftClass = left.getClass();
                Method[] declaredMethods = leftClass.getDeclaredMethods();
                methodsList = Arrays.asList(declaredMethods);
            }

            List<MethodInvokerCache> invokerList = new ArrayList<>();

            for (Method m : methodsList) {
                if (!methodName.equals(m.getName())) {
                    continue;
                }
                Class<?>[] types = m.getParameterTypes();
                if (types.length != argValues.length) {
                    continue;
                }

                ArgumentConverter[] converters = matches(m, argValues);
                if (converters == null) {
                    continue;
                }

                MethodInvokerCache methodInvokerCache = new MethodInvokerCache(leftClass, methodName, m, converters);
                invokerList.add(methodInvokerCache);

            }

            if (invokerList.size() != 1) {
                throw new RuntimeException("method with name:" + methodName + " not found");
            }
            methodInvokerCacheLocal = invokerList.get(0);
        }

        if (methodInvokerCacheLocal != null) {
            invokerCache = methodInvokerCacheLocal;
            Object result = null;
            try {
                result = methodInvokerCacheLocal.invoke(left, argValues);
            }
            catch (ReflectiveOperationException e) {
                e.printStackTrace();
            }
            if (leftHand != null) {
                evalContext.pop(1);
            }
            evalContext.push(result);
        }

        // Method method = singleMethodList.get(0);
        // try {
        // Object invoke = method.invoke(left, argValues.toArray());
        // if (leftHand!=null) {
        // evalContext.pop(1);
        // }
        // evalContext.push(invoke);
        // }
        // catch (IllegalAccessException | IllegalArgumentException | InvocationTargetException e) {
        // // TODO Auto-generated catch block
        // e.printStackTrace();
        // }
    }

    public ArgumentConverter[] matches(Method m, Object[] argValues) {
        Class<?>[] types = m.getParameterTypes();
        if (types.length != argValues.length) {
            return null;
        }

        ArgumentConverter[] result = new ArgumentConverter[argValues.length];

        for (int aidx = 0; aidx < types.length; aidx++) {
            Object argVal = argValues[aidx];
            if (argVal == null) {
                continue;
            }
            Class<?> ta = types[aidx];
            if (ta == int.class) {
                if (!(argVal instanceof Integer)) {
                    return null;
                }
            }
            else if (ta == double.class) {
                if (!(argVal instanceof Double)) {
                    return null;
                }
            }
            else if (ta == float.class) {
                if (!(argVal instanceof Float)) {
                    return null;
                }
            }
            else if (ta == char.class) {
                if (!(argVal instanceof Character)) {
                    return null;
                }
            }
            else if (ta == byte.class) {
                if (!(argVal instanceof Byte)) {
                    return null;
                }
            }
            else if (ta == short.class) {
                if (!(argVal instanceof Short)) {
                    return null;
                }
            }
            else if (ta == long.class) {
                if (!(argVal instanceof Long)) {
                    return null;
                }
            }
            else if (ta == Character.class) {
                if (!((argVal instanceof String) || (argVal instanceof Character))) {
                    return null;
                }
                result[aidx] = new OneCharStringToCharacterConverter();
            }
            else if (!types[aidx].isInstance(argVal)) {
                return null;
            }
        }

        return result;
    }

    interface ArgumentConverter {

        Object convert(Object in);
    }

    static class OneCharStringToCharacterConverter implements ArgumentConverter {

        @Override
        public Object convert(Object in) {
            if (in instanceof Character) {
                return in;
            }
            else if ((in instanceof String) && (((String) in).length() == 1)) {
                return Character.valueOf(((String) in).charAt(0));
            }
            throw new RuntimeException("Can not convert object to Character:" + in);
        }
    }

    @AllArgsConstructor
    static class MethodInvokerCache {

        private final Class<?> clazz;
        private String methodName;
        private final Method method;

        private final ArgumentConverter[] converters;

        public Object invoke(Object left, Object[] argValues) throws ReflectiveOperationException {
            for (int idx = 0; idx < converters.length; idx++) {
                if (converters[idx] == null) {
                    continue;
                }
                argValues[idx] = converters[idx].convert(argValues[idx]);
            }
            return method.invoke(left, argValues);
        }

    }

}
