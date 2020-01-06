package net.natpad.brilliance.chameleon.prime;

import net.natpad.brilliance.chameleon.ChameleonEditor;
import net.natpad.brilliance.chameleon.Document;
import net.natpad.brilliance.chameleon.uow.Uow;

public class PChameleonEditor {

	static {
        System.loadLibrary("brilliancejni");
    }

	public static native Document getDocument(ChameleonEditor chameleonEditor);

	public static native void postUow(ChameleonEditor chameleonEditor, Uow uow);

}
