package net.natpad.brilliance.chameleon.prime;

import net.natpad.brilliance.chameleon.uow.UowInsertText;
import net.natpad.brilliance.chameleon.uow.UowRemote;

public class PUow {

	static {
        System.loadLibrary("brilliancejni");
    }

	public static native long createUowRemote();

	public static native void constructUowRemote(UowRemote uowRemote, boolean isReadOnly);

	public static native long constructUowInsertText(String text);

	public static native long constructUowReplaceText(String text);

	public static native void uowInsertTextSetDelta(UowInsertText uowInsertText, int deltaRow, int deltaColumn);


}
