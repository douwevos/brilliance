package net.natpad.brilliance.leafhopper;

import net.natpad.brilliance.glibc.GObject;

public class SurfaceHandler extends GObject {

	SurfaceHandler(long id) {
		super(id);
	}

	public SurfaceBox getRoot() {
		return new SurfaceBox(Leafhopper.getRoot(this));
	}

}
