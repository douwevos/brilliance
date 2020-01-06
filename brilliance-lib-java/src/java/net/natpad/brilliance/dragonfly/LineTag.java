package net.natpad.brilliance.dragonfly;

import net.natpad.brilliance.glibc.GObject;

public class LineTag extends GObject {

	public LineTag(long row, LineTagType tagType, LineTagLevel tagLevel) {
		super(Dragonfly.createLineTag(row, tagType, tagLevel));
	}

	public void setText(String text) {
		Dragonfly.lineTagSetText(this, text);
	}
	
	public void setStartAndEndIndex(int start, int end) {
		Dragonfly.lineTagSetStartAndEndIndex(this, start, end);
	}
	
	public void setColor(double red, double green, double blue) {
		Dragonfly.lineTagSetColor(this, red, green, blue);
	}
	
}
