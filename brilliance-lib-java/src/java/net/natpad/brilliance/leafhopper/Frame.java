package net.natpad.brilliance.leafhopper;

import net.natpad.brilliance.gtk.GtkContainer;
import net.natpad.brilliance.gtk.GtkWindow;

public class Frame extends GtkContainer {

	public Frame(GtkWindow window) {
		super(Leafhopper.createFrame(window));
	}
	
	public Frame(long id) {
		super(id);
	}

	public SurfaceHandler getSurfaceHandler() {
		return new SurfaceHandler(Leafhopper.getSurfaceHandler(this));
	}
	
}
