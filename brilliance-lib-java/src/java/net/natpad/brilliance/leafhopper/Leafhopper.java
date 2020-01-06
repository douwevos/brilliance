package net.natpad.brilliance.leafhopper;

import net.natpad.brilliance.gtk.GtkWidget;
import net.natpad.brilliance.gtk.GtkWindow;

public class Leafhopper {

	static {
        System.loadLibrary("brilliancejni");
    }

	public static native long getSurfaceHandler(Frame frame);

	public static native long createSurface();

	public static native long getRoot(SurfaceHandler surfaceHandler);

	public static native void add(SurfaceBox surfaceBox, IRegion region, int index, int weight);

	public static native long surfaceGetTabModel(Surface surface);

	public static native void surfaceTabModelAddTab(SurfaceTabModel surfaceTabModel, SurfaceTab tab);

	public static native long createSurfaceTab(GtkWidget content, String label);

	public static native long createSurfaceBox(boolean horizontal);

	public static native long createFrame(GtkWindow window);

}
