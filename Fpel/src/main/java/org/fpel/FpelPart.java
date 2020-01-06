package org.fpel;


public abstract class FpelPart extends FpelNode {

    public FpelPart(FpelNode... children) {
		super(children);
	}

	public FpelPart(int left, int right) {
		super(left, right);
	}

	public abstract void eval(FpelContext context, StringBuilder out);
    
    public abstract int getStartColumn();
}
