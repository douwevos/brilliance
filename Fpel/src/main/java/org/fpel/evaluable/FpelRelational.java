package org.fpel.evaluable;

import java.util.Objects;

import org.fpel.FpelContext;
import org.fpel.FpelEvalContext;
import org.fpel.FpelEvaluable;
import org.fpel.FpelNode;
import org.fpel.FpelTokenType;

public class FpelRelational extends FpelEvaluable {

    private final FpelEvaluable left;
    private final FpelEvaluable right;
    private final FpelTokenType operator;
    
    public FpelRelational(FpelEvaluable left, FpelEvaluable right, FpelTokenType operator) {
    	super(left, right);
        this.left = left;
        this.right = right;
        this.operator = operator;
    }

    @Override
    public String name() {
    	return null;
    }

    @Override
    public FpelNode getLHS() {
    	return null;
    }
    
    public void eval(FpelContext context, FpelEvalContext evalContext) {
        left.eval(context, evalContext);
        Object leftVal = evalContext.peek();
        right.eval(context, evalContext);
        Object rightVal = evalContext.peek();
        evalContext.pop(2);
        switch(operator) {
            case EQ :
                evalContext.push(Objects.equals(leftVal, rightVal));
                break;
            case NE :
                evalContext.push(!Objects.equals(leftVal, rightVal));
                break;
            case LT : {
                int cr = doCompare(leftVal, rightVal);
                evalContext.push(cr==-1);
            } break;
            case GT : {
                int cr = doCompare(leftVal, rightVal);
                evalContext.push(cr==1);
            } break;
            case LE : {
                int cr = doCompare(leftVal, rightVal);
                evalContext.push(cr<1);
            } break;
            case GE : {
                int cr = doCompare(leftVal, rightVal);
                evalContext.push(cr>-1);
            } break;
        }
    }

    private int doCompare(Object leftVal, Object rightVal) {
        if (leftVal==rightVal) {
            return 0;
        } else if (leftVal==null) {
            return -1;
        } else if (rightVal==null) {
            return 1;
        }
        if ((leftVal instanceof Comparable) && (rightVal instanceof Comparable)) {
            Comparable u = (Comparable) leftVal;
            return u.compareTo((Comparable) rightVal);
        }
        return leftVal.toString().compareTo(rightVal.toString());
    };
}
