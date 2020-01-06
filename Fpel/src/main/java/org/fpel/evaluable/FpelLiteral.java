package org.fpel.evaluable;

import org.fpel.FpelContext;
import org.fpel.FpelEvalContext;
import org.fpel.FpelEvaluable;
import org.fpel.FpelNode;
import org.fpel.FpelToken;

public class FpelLiteral extends FpelEvaluable {

	private final FpelToken token;
    private final Object value;
    
    public FpelLiteral(FpelToken token) {
    	super(token);
    	this.token = token;
        this.value = token.getValue();
    }
    
    @Override
    public void eval(FpelContext context, FpelEvalContext evalContext) {
        evalContext.push(value);
    }
    
    @Override
    public String name() {
    	return null;
    }
    
    @Override
    public FpelNode getLHS() {
    	return null;
    }
    
}
