package net.natpad.brilliance.gtk;

import net.natpad.brilliance.glibc.GObject;

public class GtkWidget extends GObject {

	public GtkWidget(long nativeId) {
		super(nativeId);
	}
	
	public GtkWidget getParent() {
		long pid = Gtk.widgetGetParent(this);
		return pid==0l ? null : new GtkWidget(pid);
	}

	public void showAll() {
		Gtk.widgetShowAll(this);
	}
	
	public void setSizeRequest(int width, int height) {
		Gtk.widgetSetSizeRequest(this, width, height);
	}

}
