package net.natpad.brilliance.chameleon;

import net.natpad.brilliance.glibc.GObject;

public class RevisionReader extends GObject {

	protected RevisionReader(long nativeId) {
		super(nativeId);
	}
	
	public RevisionReader(Revision revision) {
		super(Chameleon.createRevisionReader(revision));
	}
	

	public int scanUtf8Char() {
		return Chameleon.revisionReaderScanUtf8Char(this);
	}
	
	
	public String readFullText() {
		StringBuilder buf = new StringBuilder();
		while(true) {
			int utf8Char = scanUtf8Char();
			if (utf8Char==-1) {
				break;
			}
			buf.appendCodePoint(utf8Char);
		}
		return buf.toString();
	}


	
}
