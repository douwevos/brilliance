package net.natpad.brilliance.chameleon.prime;

import net.natpad.brilliance.chameleon.Cursor;
import net.natpad.brilliance.chameleon.CursorM;
import net.natpad.brilliance.chameleon.Line;
import net.natpad.brilliance.chameleon.LineLocation;
import net.natpad.brilliance.chameleon.Page;
import net.natpad.brilliance.chameleon.Revision;

public class PRevision {

	static {
        System.loadLibrary("brilliancejni");
    }

	public static native CursorM toCursorM(Revision revision, Cursor cursor);

	public static native Page pageAt(Revision revision, int pageIndex);

	public static native int pageCount(Revision revision);

	public static native void setCursor(Revision revision, Cursor cursor);

	public static native Cursor getCursor(Revision revision);

	public static native Page editablePageAt(Revision revision, int pageIndex);

	public static native Line lineAtLocation(Revision revision, LineLocation location);

	public static native int insertAsLines(Revision revision, String text);

	public static native long calculateRow(Revision revision, LineLocation lineLocation);

	public static native LineLocation calculateLineLocation(Revision revision, long row);

}
