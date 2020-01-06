package org.fpel.evaluable;

import org.fpel.FpelContext;
import org.fpel.FpelEvalContext;
import org.fpel.FpelEvaluable;
import org.fpel.FpelNode;

public class FpelConditional extends FpelEvaluable {

    private final FpelEvaluable question;
    private final FpelEvaluable trueExpr;
    private final FpelEvaluable falseExpr;
    
    public FpelConditional(FpelEvaluable question, FpelEvaluable trueExpr, FpelEvaluable falseExpr) {
    	super(question, trueExpr, falseExpr);
        this.question = question;
        this.trueExpr = trueExpr;
        this.falseExpr = falseExpr;
    }
    
    @Override
    public String name() {
    	return null;
    }
    
    @Override
    public FpelNode getLHS() {
    	return question;
    }
    

    @Override
    public void eval(FpelContext context, FpelEvalContext evalContext) {
        question.eval(context, evalContext);
        Object questionResult = evalContext.peek();
        if (questionResult==null) {
            return;
        }
        evalContext.pop(1);
        Boolean result = null;
        if (questionResult instanceof Boolean) {
            result = (Boolean) questionResult;
        } else if (questionResult instanceof String) {
           String v = (String) questionResult;
           if (v.equalsIgnoreCase("true")) {
               result = Boolean.TRUE;
           } else if (v.equalsIgnoreCase("false")) {
               result = Boolean.FALSE;
           } 
        }
        
        if (result==Boolean.TRUE) {
            trueExpr.eval(context, evalContext);
        } else if (result==Boolean.FALSE) {
            falseExpr.eval(context, evalContext);
        } else {
            evalContext.push(null);
        }
    }
}
