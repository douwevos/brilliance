package org.fpel;


public abstract class FpelEvaluable extends FpelNode {
	
    public FpelEvaluable(FpelNode... children) {
		super(children);
	}

	public FpelEvaluable(int left, int right) {
		super(left, right);
	}

	public abstract void eval(FpelContext context, FpelEvalContext evalContext);
 
	public abstract String name();
	
	public abstract FpelNode getLHS(); 
    
}
