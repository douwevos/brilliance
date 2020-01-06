package net.natpad.brilliance.chameleon;

public class Cursor {

	private final LineLocation lineLocation;
	private final int byteOffset;
	private final int subOffset;
	
	public Cursor(LineLocation lineLocation, int byteOffset, int subOffset) {
		this.lineLocation = lineLocation;
		this.byteOffset = byteOffset;
		this.subOffset = subOffset;
	}
	
	
	public LineLocation getLineLocation() {
		return lineLocation;
	}
	
	public int getSubOffset() {
		return subOffset;
	}
	
	public int getByteOffset() {
		return byteOffset;
	}


	@Override
	public String toString() {
		return "Cursor[lineLocation=" + lineLocation + ", byteOffset=" + byteOffset + ", subOffset=" + subOffset + "]";
	}
	
}
