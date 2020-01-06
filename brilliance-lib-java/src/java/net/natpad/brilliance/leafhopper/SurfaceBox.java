package net.natpad.brilliance.leafhopper;

import net.natpad.brilliance.glibc.GObject;

public class SurfaceBox extends GObject implements IRegion {

	public static enum Orientation {
		VERTICAL, HORIZONTAL
	}
	
	public SurfaceBox(Orientation orientation) {
		super(Leafhopper.createSurfaceBox(orientation==Orientation.HORIZONTAL));
	}
	
	public SurfaceBox(long nativeId) {
		super(nativeId);
	}

	public void add(IRegion region, int index, int weight) {
		Leafhopper.add(this, region, index, weight);
	}
	
}
