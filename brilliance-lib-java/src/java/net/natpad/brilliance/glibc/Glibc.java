package net.natpad.brilliance.glibc;

public class Glibc {

	static {
        System.loadLibrary("brilliancejni");
    }

	public static native void objectUnref(long nativeId);

}
