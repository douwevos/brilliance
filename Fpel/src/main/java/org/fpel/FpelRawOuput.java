package org.fpel;

import java.util.stream.Stream;

public class FpelRawOuput extends FpelPart {

	private final int columnAt;
    private final String text;
    
    public FpelRawOuput(int columnAt, String text) {
    	super(0, text.length());
    	this.columnAt = columnAt;
    	this.text = text;
    }
    
    @Override
    public void eval(FpelContext context, StringBuilder out) {
        out.append(text);
    }
 
    @Override
    public Stream<FpelNode> streamChildren() {
    	return Stream.empty();
    }

    @Override
    public int getStartColumn() {
    	return columnAt;
    }
    
    @Override
    public int getLeft() {
    	return 0;
    }
    
    @Override
    public int getRight() {
    	return text.length();
    }

	@Override
	public String toString() {
		return "FpelRawOuput [columnAt=" + columnAt + ", text=" + text + "]";
	}
    
    
    
}
