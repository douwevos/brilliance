package net.natpad.brilliance.dragonfly;

import net.natpad.brilliance.chameleon.Document;
import net.natpad.brilliance.gtk.GtkWidget;

public class DocumentView extends net.natpad.brilliance.chameleon.DocumentView {

	public DocumentView(Document document, long pango_context, GtkWidget widget, long info_key) {
		super(Dragonfly.createDocumentView());
		Dragonfly.constructDocumentView(this, document, pango_context, widget, info_key);
	}
	
}
