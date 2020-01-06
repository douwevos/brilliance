package net.natpad.brilliance.treeviewer;

public class TreeViewerNative {

	static {
        System.loadLibrary("brilliancejni");
    }

	public static native long createTreeViewer();

}
