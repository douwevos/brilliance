package net.natpad.brilliance.dragonfly;

import net.natpad.brilliance.chameleon.ChameleonEditor;
import net.natpad.brilliance.chameleon.Document;
import net.natpad.brilliance.dragonfly.prime.PDragonflyEditor;
import net.natpad.brilliance.gtk.GtkWidget;
import net.natpad.brilliance.worm.Worm;

public class DragonflyEditor extends ChameleonEditor {

	DocumentView documentView;
	
	DragonflyEditor(long nativeId) {
		super(nativeId);
	}
	
	public DragonflyEditor(Document document, ConnectorMap connectorMap, IConnectorRequestFactory connectorFactory) {
		super(Dragonfly.createEditor());
		Dragonfly.constructEditor(this, document, connectorMap, connectorFactory, Worm.getWorkerService());
	}

	public void setContextEditor(ContextEditor contextEditor) {
		System.err.println("invoking native");
		Dragonfly.editorSetContextEditor(this, contextEditor);
	}

	
	@Override
	public DocumentView initializeDocumentView(Document document, long pango_context, GtkWidget widget) {
		documentView = new DocumentView(document, pango_context, widget, getLineInfoKey());;
		return documentView;
	}

	private long getLineInfoKey() {
		return Dragonfly.editorGetLineInfoKey(this);
	}
	
	public void runUowInsertTemplateRaw(String text) {
		Dragonfly.editorUowInsertTemplateRaw(this, text);
	}

	public Document getDocument() {
		return PDragonflyEditor.getDocument(this);
	}
}
