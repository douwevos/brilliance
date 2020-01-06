package net.natpad.brilliance.dragonfly;

import net.natpad.brilliance.Logger;
import net.natpad.brilliance.chameleon.ChameleonEditor;
import net.natpad.brilliance.chameleon.Cursor;
import net.natpad.brilliance.chameleon.Revision;
import net.natpad.brilliance.chameleon.RevisionReader;
import net.natpad.brilliance.glibc.GObject;

public class AcContext extends GObject implements Logger {

	public AcContext(long nativeId) {
		super(nativeId);
	}
	
	
	public RevisionReader createRevisionReader() {
		return Dragonfly.acContextCreateRevisionReader(this);
	}
	
	public Revision getRevision() {
		return Dragonfly.acContextGetRevision(this);
	}
	
	
	public Cursor getEndCursor() {
		return Dragonfly.acContextGetEndCursor(this);
	}

	public Cursor getStartCursor() {
		return Dragonfly.acContextGetStartCursor(this);
	}

	public Cursor getFilterCursor() {
		return Dragonfly.acContextGetFilterCursor(this);
	}

	
	public void addEntry(AcEntry entry) {
		Dragonfly.acContextAddEntry(this, entry);
	}
	
	public ChameleonEditor getEditor() {
		log("kom ik hier wel !!");
		return Dragonfly.acContextGetEditor(this);
	}
	
}
