package net.natpad.brilliance.worm;

import net.natpad.brilliance.glibc.GObject;

public class WorkerService extends GObject {

	public WorkerService() {
		super(Worm.createWorkerService());
	}
	
	public WorkerService(long nativeId) {
		super(nativeId);
	}

	public void postRequest(Request request) {
		Worm.workerServicePostRequest(Worm.getWorkerService(), request);
	}
}
