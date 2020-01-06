package net.natpad.brilliance.chameleon;

import net.natpad.brilliance.glibc.GObject;

public class Page extends GObject {

	protected Page(long nativeId) {
		super(nativeId);
	}

	public void holdLines() {
		Chameleon.pageHoldLines(this);
	}

	public void releaseLines() {
		Chameleon.pageReleaseLines(this);
		
	}

	public Line lineAt(int pageLineIndex) {
		return Chameleon.pageLineAt(this, pageLineIndex);
	}
	
}
