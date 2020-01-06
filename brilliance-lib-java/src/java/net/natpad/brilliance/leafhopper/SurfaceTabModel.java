package net.natpad.brilliance.leafhopper;

import net.natpad.brilliance.glibc.GObject;

public class SurfaceTabModel extends GObject {

	
	public SurfaceTabModel(long nativeId) {
		super(nativeId);
	}
	
	public void addTab(SurfaceTab tab) {
		Leafhopper.surfaceTabModelAddTab(this, tab);
	}
}
