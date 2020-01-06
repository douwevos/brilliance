package org.fpel.evaluable;

import java.lang.reflect.Field;
import java.lang.reflect.Method;

import org.fpel.FpelContext;
import org.fpel.FpelEvalContext;
import org.fpel.FpelEvaluable;
import org.fpel.FpelNode;
import org.fpel.FpelToken;

import lombok.AllArgsConstructor;

public class FpelGetProperty extends FpelEvaluable {

    private final FpelEvaluable evaluable;
    private final FpelToken keyToken;
    private final String key;

    private volatile Invoker invoker;
    
    public FpelGetProperty(FpelEvaluable evaluable, FpelToken keyToken) {
    	super(evaluable, keyToken);
        this.evaluable = evaluable;
        this.keyToken = keyToken;
        this.key = (String) keyToken.getValue();
    }
    
    @Override
    public String name() {
    	return (String) keyToken.getValue();
    }
    
    @Override
    public FpelNode getLHS() {
    	return evaluable;
    }
    
    public FpelEvaluable getEvaluable() {
		return evaluable;
	}
    
    public String getName() {
    	return keyToken==null ? null : (String) keyToken.getValue();
    }
    
    @Override
    public void eval(FpelContext context, FpelEvalContext evalContext) {
        if (evaluable != null) {
            evaluable.eval(context, evalContext);
            Object top = evalContext.peek();
            if (top == null) {
                return;
            }
            evalContext.pop(1);
            
            
            Invoker invokerLocal = invoker;
            
            Class<? extends Object> class1 = top.getClass();
            if (invokerLocal!=null && !invokerLocal.matches(class1)) {
                invokerLocal = null;
//                invoker = null;
            }
            
            if (invokerLocal==null) {
                Class<? extends Object> classIter = class1;
                
                while(classIter!=null && invokerLocal==null) {
                    try {
                        Field field = classIter.getDeclaredField(key);
                        if (field != null) {
                            field.setAccessible(true);
                            invokerLocal = new FieldInvoker(class1, field);
                        }
                    }
                    catch (NoSuchFieldException | SecurityException e) {
                    }
                    classIter = classIter.getSuperclass();
                }
            }
            
            if (invokerLocal==null) {
                Class<? extends Object> classIter = class1;
                String getMethodName = "get" + Character.toUpperCase(key.charAt(0)) + key.substring(1);
                while(classIter!=null && invokerLocal==null) {
                    try {
                        Method method = classIter.getDeclaredMethod(getMethodName);
                        method.setAccessible(true);
                        invokerLocal = new MethodInvoker(class1, method);
                    }
                    catch (NoSuchMethodException | SecurityException e) {
                    }
                    classIter = classIter.getSuperclass();
                }
            }

            if (invokerLocal==null) {
                invokerLocal = new NullInvoker(class1);
            }
            

            if (invokerLocal!=null) {
                invoker = invokerLocal;
                try {
                    Object result = invokerLocal.invoke(top);
                    evalContext.push(result);
                    return;
                }
                catch (ReflectiveOperationException e) {
                    e.printStackTrace();
                }
            }
            
            evalContext.push(null);
        }
        else {
            Object object = context.get(key);
            evalContext.push(object);
        }
    }

    interface Invoker {

        boolean matches(Class<?> clazz);
        Object invoke(Object left) throws ReflectiveOperationException;
    }

    @AllArgsConstructor
    static class MethodInvoker implements Invoker {

        private final Class<?> clazz;
        private final Method method;
        
        @Override
        public Object invoke(Object left) throws ReflectiveOperationException {
            return method.invoke(left);
        }

        @Override
        public boolean matches(Class<?> clazz) {
            return this.clazz == clazz;
        }

    }

    @AllArgsConstructor
    static class FieldInvoker implements Invoker {
        private final Class<?> clazz;
        private final Field field;
        
        @Override
        public Object invoke(Object left) throws ReflectiveOperationException {
            return field.get(left);
        }

        @Override
        public boolean matches(Class<?> clazz) {
            return this.clazz == clazz;
        }
    }

    
    @AllArgsConstructor
    static class NullInvoker implements Invoker {
        private final Class<?> clazz;
        
        @Override
        public Object invoke(Object left) throws ReflectiveOperationException {
            return null;
        }
        
        @Override
        public boolean matches(Class<?> clazz) {
            return this.clazz == clazz;
        }
    }
    
    @Override
    public String toString() {
    	if (evaluable == null) {
    		return "FpelGetProperty["+key+"]";
    	}
    	return "FpelGetProperty[" + evaluable.getClass().getName()+  ", "+key+"]";
    }

    
}
