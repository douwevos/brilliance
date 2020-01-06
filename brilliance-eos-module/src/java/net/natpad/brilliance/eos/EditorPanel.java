package net.natpad.brilliance.eos;

import net.natpad.brilliance.chameleon.Document;
import net.natpad.brilliance.dragonfly.ConnectorMap;
import net.natpad.brilliance.dragonfly.DragonflyEditor;
import net.natpad.brilliance.dragonfly.IConnectorRequestFactory;
import net.natpad.brilliance.leafhopper.IPanelOwner;
import net.natpad.brilliance.worm.WorkerService;

public class EditorPanel extends net.natpad.brilliance.dragonfly.EditorPanel {

	public EditorPanel(IPanelOwner panelOwner, Document document, IConnectorRequestFactory connectorFactory) {
		super(panelOwner, document, connectorFactory);
	}
	
	@Override
	public DragonflyEditor initEditor(Document document, ConnectorMap connectorMap,
			IConnectorRequestFactory requestFactory, WorkerService workerService) {
		dragonflyEditor = new Editor(document, connectorMap, requestFactory);
		return dragonflyEditor;
	}
	
	
	
	
}
