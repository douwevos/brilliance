package net.natpad.brilliance.worm;

import net.natpad.brilliance.Logger;

public class Worm implements Logger {

	static {
        System.loadLibrary("brilliancejni");
    }

	private static WorkerService workerService;
	
	public static void setupWormService(long nativeId) {
		workerService = new WorkerService(nativeId);
	}

	public static void setupWormService() {
		workerService = new WorkerService();
	}

	
	public static WorkerService getWorkerService() {
		return workerService;
	}

	public static native long createRequest();

	public static native void requestLink(Request request);

	public static native void workerServicePostRequest(WorkerService workerService, Request request);

	public static native long createWorkerService();
	
}
