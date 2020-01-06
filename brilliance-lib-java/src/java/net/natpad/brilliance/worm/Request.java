package net.natpad.brilliance.worm;

import net.natpad.brilliance.glibc.GObject;

public abstract class Request extends GObject {

	protected Request(long nativeId) {
		super(nativeId);
	}
	
	protected Request() {
		super(Worm.createRequest());
		Worm.requestLink(this);
	}
	
	
	public abstract void runRequest();
	
}
