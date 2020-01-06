package net.natpad.brilliance.chameleon;

public class CursorM {

	private final long row;
	private final int column;
	
	public CursorM(long row, int column) {
		this.row = row;
		this.column = column;
	}
	
	public long getRow() {
		return row;
	}
	
	public int getColumn() {
		return column;
	}

	@Override
	public String toString() {
		return "CursorM[row=" + row + ", column=" + column + "]";
	}
}
