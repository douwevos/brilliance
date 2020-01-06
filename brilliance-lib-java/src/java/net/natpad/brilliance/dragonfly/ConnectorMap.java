package net.natpad.brilliance.dragonfly;

import net.natpad.brilliance.glibc.GObject;
import net.natpad.brilliance.worm.WorkerService;

public class ConnectorMap extends GObject {

	private ConnectorMap(long nativeId) {
		super(nativeId);
	}

	
	public ConnectorMap(WorkerService workerService) {
		super(Dragonfly.createConnectorMap(workerService));
	}
}
