package net.natpad.brilliance.leafhopper;

import net.natpad.brilliance.gtk.GtkWidget;

public class Surface extends GtkWidget implements IRegion {
	
	public Surface() {
		super(Leafhopper.createSurface());
	}

	public SurfaceTabModel getTabModel() {
		long id = Leafhopper.surfaceGetTabModel(this);
		return id==0 ? null : new SurfaceTabModel(id);
	}
	
}
