package net.natpad.brilliance.chameleon;

import net.natpad.brilliance.glibc.GObject;

public final class Line extends GObject {

	protected Line(long nativeId) {
		super(nativeId);
	}

	public String getText() {
		return Chameleon.lineGetText(this);
	}
	
}
