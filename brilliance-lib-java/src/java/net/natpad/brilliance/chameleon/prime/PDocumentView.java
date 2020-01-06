package net.natpad.brilliance.chameleon.prime;

import net.natpad.brilliance.chameleon.DocumentView;

public class PDocumentView {

	static {
        System.loadLibrary("brilliancejni");
    }

	public static native boolean removeSelection(DocumentView documentView);

	public static native void moveViewToFocus(DocumentView documentView, boolean do_center);

}
