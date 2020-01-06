package net.natpad.brilliance.chameleon.uow;

import net.natpad.brilliance.chameleon.ChameleonEditor;
import net.natpad.brilliance.chameleon.DocumentView;
import net.natpad.brilliance.chameleon.prime.PUow;

public abstract class UowRemote extends Uow {

	public UowRemote(boolean isReadOnly) {
		super(PUow.createUowRemote());
		PUow.constructUowRemote(this, isReadOnly);
	}
	
	public abstract void run(ChameleonEditor editor, DocumentView documentView);
}
