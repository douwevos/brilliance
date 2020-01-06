package org.fpel;

import java.util.stream.Stream;

import lombok.Getter;

@Getter
public class FpelToken extends FpelNode {

    private final FpelTokenType type;
    private final Object value;
    private final int left;
    private final int right;
    
    public FpelToken(FpelTokenType type, Object value, int left, int right) {
    	super(left, right);
        this.type = type;
        this.value = value;
        this.left = left;
        this.right = right;
    }

    public FpelToken(FpelTokenType type) {
    	super(-1, -1);
        this.type = type;
        this.value = null;
        left = -1;
        right = -1;
    }
    
    @Override
    public Stream<FpelNode> streamChildren() {
    	return Stream.empty();
    }

	@Override
	public String toString() {
		return "FpelToken [type=" + type + ", value=" + value + ", left=" + left + ", right=" + right + "]";
	}
    
}
