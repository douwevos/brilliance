package net.natpad.brilliance.chameleon;

import java.io.File;

import net.natpad.brilliance.glibc.GObject;

public class DocumentManager extends GObject {

	public DocumentManager() {
		super(Chameleon.createDocumentManager());
	}
	
	public DocumentManager(long nativeId) {
		super(nativeId);
	}
	
	public void load(Document document, File file) {
		String path = file.getPath();
		Chameleon.documentManagerLoad(this, document, path);
	}

}
