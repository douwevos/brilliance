package net.natpad.brilliance.caterpillar;

public class StringWo {

	static {
        System.loadLibrary("brilliancejni");
    }

	private final long id;
	
	public StringWo() {
		id = createString();
	}
	
	@Override
	protected void finalize() throws Throwable {
		super.finalize();
		destroyString(id);
		
	}
	
	private native long createString();
	private native void destroyString(long id);

	
	@Override
	public String toString() {
		return "StringWo[id="+id+"]";
	}
}
