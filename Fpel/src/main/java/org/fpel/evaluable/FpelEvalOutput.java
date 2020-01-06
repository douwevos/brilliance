package org.fpel.evaluable;

import org.fpel.FpelContext;
import org.fpel.FpelEvalContext;
import org.fpel.FpelEvaluable;
import org.fpel.FpelPart;

public class FpelEvalOutput extends FpelPart {

	private final int startColumn;
    private final FpelEvaluable evaluable;
    
    public FpelEvalOutput(int startColumn, FpelEvaluable evaluable) {
    	super(evaluable);
    	this.startColumn = startColumn;
        this.evaluable = evaluable;
    }

    public FpelEvalOutput(int startColumn) {
    	super(0, 0);
    	this.startColumn = startColumn;
    	evaluable = null;
    }

    @Override
    public int getStartColumn() {
    	return startColumn;
    }

    @Override
    public void eval(FpelContext context, StringBuilder out) {
        FpelEvalContext evalContext = new FpelEvalContext();
        evaluable.eval(context, evalContext);
        Object result = evalContext.getResult();
        if (result!=null) {
            out.append(result.toString());
        }
    }

	@Override
	public String toString() {
		return "FpelEvalOutput [startColumn=" + startColumn + ", evaluable=" + evaluable + "]";
	}
    
}
