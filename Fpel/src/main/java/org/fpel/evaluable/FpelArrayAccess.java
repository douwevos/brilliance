package org.fpel.evaluable;

import java.lang.reflect.Array;
import java.util.List;
import java.util.Map;

import org.fpel.FpelContext;
import org.fpel.FpelEvalContext;
import org.fpel.FpelEvaluable;
import org.fpel.FpelNode;
import org.fpel.FpelToken;

public class FpelArrayAccess extends FpelEvaluable {

    private final FpelEvaluable field;
    private final FpelEvaluable fieldArrayOffset;
    
    public FpelArrayAccess(FpelEvaluable field, FpelToken leftSqBr, FpelEvaluable fieldArrayOffset, FpelToken rightSqBr) {
    	super(field, leftSqBr, fieldArrayOffset, rightSqBr);
        this.field = field;
        this.fieldArrayOffset = fieldArrayOffset;
    }

    public FpelEvaluable getField() {
		return field;
	}
    
    public FpelEvaluable getFieldArrayOffset() {
		return fieldArrayOffset;
	}
    
    @Override
    public FpelNode getLHS() {
    	return field;
    }
    

    @SuppressWarnings("rawtypes")
	@Override
    public void eval(FpelContext context, FpelEvalContext evalContext) {
        field.eval(context, evalContext);
        Object result = evalContext.peek();
        if (result!=null) {
            fieldArrayOffset.eval(context, evalContext);
            Object resultOffset = evalContext.peek();
            
            Object newVal = null;
            evalContext.pop(2);
            if (resultOffset!=null) {
                if (result instanceof Map) {
                    newVal = ((Map) result).get(resultOffset);
                } else if (resultOffset instanceof Number) {
                    int offset = ((Number) resultOffset).intValue();
                    if (result instanceof List) {
                        List l = (List) result;
                        if (offset>=0 && offset<l.size()) {
                            newVal = l.get(offset);
                        }
                    } else if (result.getClass().isArray()) {
                        newVal = Array.get(result, offset);
                    }
                }
            }
            evalContext.push(newVal);
        }
    }
    
    @Override
    public String name() {
    	return field.name();
    }
    
}
