package net.natpad.brilliance.chameleon;

import net.natpad.brilliance.chameleon.prime.PChameleonEditor;
import net.natpad.brilliance.chameleon.uow.Uow;
import net.natpad.brilliance.chameleon.uow.UowInsertText;
import net.natpad.brilliance.gtk.GtkWidget;

public abstract class ChameleonEditor extends GtkWidget {

	
	protected ChameleonEditor(long nativeId) {
		super(nativeId);
	}

	public ChameleonEditor(Document document) {
		super(Chameleon.createEditor(document));
	}
	
	public abstract DocumentView initializeDocumentView(Document document, long pango_context, GtkWidget widget);

	public DocumentView getDocumentView() {
		return Chameleon.editorGetDocumentView(this);
	}

	public boolean gotoLine(long line) {
		return Chameleon.editorGotoLine(this, line);
	}
	
	public void postUowInsertText(String text) {
		Chameleon.editorPostUowInsertText(this, text);
	}

	public Uow createInsertOrReplaceUow(int edit_mode, String text) {
		return new UowInsertText(text);
	}
	

	public Document getDocument() {
		return PChameleonEditor.getDocument(this);
	}
	
	public void postUow(Uow uow) {
		PChameleonEditor.postUow(this, uow);
	}
}
