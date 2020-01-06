package net.natpad.brilliance.dragonfly.prime;

import net.natpad.brilliance.chameleon.Document;
import net.natpad.brilliance.dragonfly.DragonflyEditor;

public class PDragonflyEditor {

	static {
        System.loadLibrary("brilliancejni");
    }

	// TODO move to Chameleon
	public static native Document getDocument(DragonflyEditor dragonflyEditor);
	
}
