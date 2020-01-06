package org.fpel;

import java.lang.reflect.Method;
import java.util.HashMap;
import java.util.Map;

public class FpelContext {

    private Map<String, Object> properties = new HashMap<>();

    private Map<String, Method> functions = new HashMap<>();

    public void put(String key, Object value) {
        properties.put(key, value);
    }
    
    public Object get(String key) {
        return properties.get(key);
    }
    
    public void definedFunction(String functionName, Method method) {
        functions.put(functionName, method);
    }
    
    public Method getFunction(String functionName) {
        return functions.get(functionName);
    }

}
