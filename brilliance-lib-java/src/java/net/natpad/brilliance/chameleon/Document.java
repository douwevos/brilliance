package net.natpad.brilliance.chameleon;

import net.natpad.brilliance.chameleon.DocumentListenerHooks.Hook;
import net.natpad.brilliance.chameleon.prime.PDocument;
import net.natpad.brilliance.glibc.GObject;

public final class Document extends GObject {

	private Hook hook;
	
	private Document(long nativeId) {
		super(nativeId);
		hook = DocumentListenerHooks.instance().refHook(getNativeId());
	}
	
	public Document(DocumentManager manager) {
		super(Chameleon.createDocument(manager));
		hook = DocumentListenerHooks.instance().refHook(getNativeId());
	}
	
	@Override
	protected void finalize() throws Throwable {
		if (hook!=null) {
			DocumentListenerHooks.instance().unrefHook(hook);
		}
		hook = null;
	}
	
	public void addListener(IDocumentListener listener) {
		hook.add(listener);
	}

	public Revision getCurrentRevision() {
		return Chameleon.documentGetCurrentRevision(this);
	}
	
	public Revision getEditableRevision() {
		return PDocument.getEditableRevision(this);
	}

	public boolean isEditable() {
		return PDocument.isEditable(this);
	}

	public void anchorDocument() {
		PDocument.anchorDocument(this);
	}
}
