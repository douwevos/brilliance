package net.natpad.brilliance.chameleon.uow;

import net.natpad.brilliance.chameleon.prime.PUow;

public class UowInsertText extends Uow {

	public UowInsertText(String text) {
		super(PUow.constructUowInsertText(text));
	}

	public UowInsertText(String text, int deltaRow, int deltaColumn) {
		super(PUow.constructUowInsertText(text));
		PUow.uowInsertTextSetDelta(this, deltaRow, deltaColumn);
	}

}
