package net.natpad.brilliance.chameleon;

public class LineLocation {

	private final int pageIndex;
	private final int pageLineIndex;
	
	public LineLocation(int pageIndex, int pageLineIndex) {
		this.pageIndex = pageIndex;
		this.pageLineIndex = pageLineIndex;
	}
	
	public int getPageIndex() {
		return pageIndex;
	}
	
	public int getPageLineIndex() {
		return pageLineIndex;
	}

	@Override
	public String toString() {
		return "LineLocation[pageIndex=" + pageIndex + ", pageLineIndex=" + pageLineIndex + "]";
	}
	
}
