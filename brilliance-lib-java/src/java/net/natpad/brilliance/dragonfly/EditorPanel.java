package net.natpad.brilliance.dragonfly;

import net.natpad.brilliance.Logger;
import net.natpad.brilliance.chameleon.Document;
import net.natpad.brilliance.leafhopper.IPanelOwner;
import net.natpad.brilliance.leafhopper.Panel;
import net.natpad.brilliance.worm.WorkerService;

public class EditorPanel extends Panel implements Logger {

	protected DragonflyEditor dragonflyEditor;

	private String name;
	
	public EditorPanel(IPanelOwner panelOwner, Document document, IConnectorRequestFactory connectorFactory) {
		super(Dragonfly.createEditorPanel());
		Dragonfly.constructEditorPanelLink(this, panelOwner, document, connectorFactory);
	}
	
	public DragonflyEditor getEditor() {
		long nativeId = Dragonfly.editorPanelGetEditor(this);
		return new DragonflyEditor(nativeId);
	}
	
	public DragonflyEditor initEditor(Document document, ConnectorMap connectorMap, IConnectorRequestFactory requestFactory, WorkerService workerService) {
		log("########### ga nu eein editor openen");
		dragonflyEditor = new DragonflyEditor(document, connectorMap, requestFactory);
		return dragonflyEditor;
	}

	public String getName() {
		return name;
	}
	
	public void setName(String name) {
		this.name = name;
	}
}
