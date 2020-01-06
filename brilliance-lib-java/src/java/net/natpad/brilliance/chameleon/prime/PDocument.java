package net.natpad.brilliance.chameleon.prime;

import net.natpad.brilliance.chameleon.Document;
import net.natpad.brilliance.chameleon.Revision;

public class PDocument {

	static {
        System.loadLibrary("brilliancejni");
    }

	public static native Revision getEditableRevision(Document document);

	public static native boolean isEditable(Document document);

	public static native void anchorDocument(Document document);
	
}
