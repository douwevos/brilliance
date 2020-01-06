package net.natpad.brilliance.caterpillar;

public class Caterpillar {

	static {
        System.loadLibrary("brilliancejni");
    }

	public static native void referenceReport();

	
}
