package org.fpel;

import java.util.ArrayList;
import java.util.List;

public class FpelEvalContext {

    private List<Object> stack = new ArrayList<>();
    
    public void push(Object object) {
        stack.add(object);
    }

    public Object getResult() {
        if (stack.size()>0) {
            return stack.get(0);
        }
        return null;
    }

    public Object peek() {
        return stack.get(stack.size()-1);
    }

    public void pop(int count) {
        while(count>0) {
            stack.remove(stack.size()-1);
            count--;
        }
    }
    
}
