package net.natpad.brilliance.gtk;

public class GtkContainer extends GtkWidget {

	protected GtkContainer(long nativeId) {
		super(nativeId);
	}
	
	
	public void add(GtkWidget child) {
		Gtk.containerAdd(this, child);
	}
	
	
}
