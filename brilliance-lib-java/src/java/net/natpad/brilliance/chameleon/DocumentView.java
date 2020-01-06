package net.natpad.brilliance.chameleon;

import net.natpad.brilliance.chameleon.prime.PDocumentView;
import net.natpad.brilliance.glibc.GObject;
import net.natpad.brilliance.gtk.GtkWidget;

public class DocumentView extends GObject {

	protected DocumentView(long nativeId) {
		super(nativeId);
	}
	
	public DocumentView(Document document, long pango_context, GtkWidget widget) {
		super(Chameleon.createDocumentView());
		Chameleon.constructDocumentView(this, document, pango_context, widget);
	}
	
	public void setPlainSelection(Cursor startCursor, Cursor filterCursor) {
		Chameleon.documentViewSetPlainSelection(this, startCursor, filterCursor);
	}
	
	public boolean removeSelection() {
		return PDocumentView.removeSelection(this);
	}

	public void moveViewToFocus(boolean do_center) {
		PDocumentView.moveViewToFocus(this, do_center);
	}
}
