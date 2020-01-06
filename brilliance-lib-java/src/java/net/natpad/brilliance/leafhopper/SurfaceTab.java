package net.natpad.brilliance.leafhopper;

import net.natpad.brilliance.glibc.GObject;
import net.natpad.brilliance.gtk.GtkWidget;

public class SurfaceTab extends GObject {

	public SurfaceTab(GtkWidget widget, String label) {
		super(Leafhopper.createSurfaceTab(widget, label));
	}

}
