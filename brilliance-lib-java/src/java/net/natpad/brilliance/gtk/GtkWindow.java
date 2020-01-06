package net.natpad.brilliance.gtk;

public class GtkWindow extends GtkBin {

	
	public GtkWindow() {
		super(Gtk.createWindow());
	}
	
	protected GtkWindow(long nativeId) {
		super(nativeId);
	}
	
}
