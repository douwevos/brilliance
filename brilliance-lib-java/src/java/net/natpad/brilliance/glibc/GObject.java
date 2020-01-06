package net.natpad.brilliance.glibc;

import net.natpad.brilliance.Logger;

public class GObject implements Logger {

	protected long id;
	
	public GObject(long nativeId) {
		this.id = nativeId;
	}
	
	public long getNativeId() {
		return id;
	}
	
	@Override
	protected void finalize() throws Throwable {
		super.finalize();
		log("finalizing:"+this);
		long t = id;
		id = 0;
		if (t != 0) {
			Glibc.objectUnref(t);
		}
	}
	
}
