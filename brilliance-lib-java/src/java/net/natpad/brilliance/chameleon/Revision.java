package net.natpad.brilliance.chameleon;

import net.natpad.brilliance.chameleon.prime.PRevision;
import net.natpad.brilliance.glibc.GObject;

public class Revision extends GObject {

	
	public Revision(long nativeId) {
		super(nativeId);
	}
	
	public CursorM toCursorM(Cursor cursor) {
//		return Dragonfly.revisionToCursorM(this, cursor);
		return PRevision.toCursorM(this, cursor);
	}
	
	public void setCursor(Cursor cursor) {
		PRevision.setCursor(this, cursor);
	}
	
	public Cursor getCursor() {
		return PRevision.getCursor(this);
	}
	
	public long calculateRow(LineLocation lineLocation) {
		return PRevision.calculateRow(this, lineLocation);
	}

	public LineLocation calculateLineLocation(long row) {
		return PRevision.calculateLineLocation(this, row);
	}

	public Page pageAt(int pageIndex) {
//		return Chameleon.revisionPageAt(this, pageIndex);
		return PRevision.pageAt(this, pageIndex);
	}

	public Page editablePageAt(int pageIndex) {
		return PRevision.editablePageAt(this, pageIndex);
	}

	public Line lineAtLocation(LineLocation location) {
		return PRevision.lineAtLocation(this, location);
	}
	
	public int pageCount() {
		return PRevision.pageCount(this);
	}
	
	public int insertAsLines(String text) {
		return PRevision.insertAsLines(this, text);
	}
	
}
