package org.fpel;

import java.util.ArrayList;
import java.util.List;

import org.fpel.evaluable.FpelEvalOutput;

import lombok.Builder;

@Builder
public class FpelExpression {

    private final List<FpelPart> parts;

    private FpelExpression(List<FpelPart> parts) {
        int count = 0;
        if (parts != null) {
            count = parts.size();
        }
        this.parts = new ArrayList<>(count);
        this.parts.addAll(parts);
    }
    
	public void dump() {
		for(FpelPart part : parts) {
			dump(part, "");
		}
	}

    private void dump(FpelNode part, String prefix) {
    	System.out.println(prefix+part);
    	String sp = prefix + "   ";
		part.streamChildren().forEach(s -> dump(s, sp));
	}

	public String eval(FpelContext context) {
        final StringBuilder buf = new StringBuilder();
        parts.stream().forEach(p -> p.eval(context, buf));
        return buf.toString();
    }

    
    public boolean isAtColumn(FpelNode node, int column) {
    	return node.getLeft()<=column && node.getRight()>=column;
    }
    
	public List<FpelNode> pathToLocation(int actualColumn) {
		List<FpelNode> result = new ArrayList<FpelNode>();
		FpelPart fpelPart = null;
		for(FpelPart part : parts) {
			if (part.getStartColumn()>actualColumn) {
				break;
			}
			fpelPart = part;
		}
		System.out.println("fpelPart="+fpelPart);
//		FpelPart fpelPart = parts.stream().filter(p -> isAtColumn(p, actualColumn)).findFirst().orElse(null);
		if (fpelPart != null) {
			result.add(fpelPart);
			if (fpelPart instanceof FpelEvalOutput) {
				pathToLocation(result, (FpelEvalOutput) fpelPart, actualColumn-2-fpelPart.getStartColumn());
			}
		}
		return result;
	}

    

    private void pathToLocation(List<FpelNode> nodeList, FpelEvalOutput fpelPart, int column) {
    	FpelNode mainNode = fpelPart;
    	while(mainNode != null) {
	    	FpelNode found = mainNode.streamChildren().filter(p -> isAtColumn(p, column)).findAny().orElse(null);
	    	if (found != null) {
	    		nodeList.add(found);
	    		mainNode = found;
	    	} else {
	    		mainNode = null;
	    	}
    	}    	
	}



	public static class FpelExpressionBuilder {

        public void addPart(FpelPart fpelPart) {
            if (parts == null) {
                parts = new ArrayList<>();
            }
            parts.add(fpelPart);
        }

    }

}
