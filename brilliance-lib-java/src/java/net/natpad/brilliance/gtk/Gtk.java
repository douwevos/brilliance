package net.natpad.brilliance.gtk;

public class Gtk {

	static {
        System.loadLibrary("brilliancejni");
    }

	public static native void containerAdd(GtkContainer container, GtkWidget child);
	
	public static native void destroy(long id);

	public static native long widgetGetParent(GtkWidget gtkWidget);
	public static native void widgetShowAll(GtkWidget gtkWidget);
	public static native void widgetSetSizeRequest(GtkWidget gtkWidget, int width, int height);

	public static native long createScrolledWindow();

	public static native long createWindow();


	public static native void main();
	
}
