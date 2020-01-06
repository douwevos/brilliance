package net.natpad.brilliance.chameleon.uow;

import net.natpad.brilliance.chameleon.prime.PUow;

public class UowReplaceText extends Uow {

	public UowReplaceText(String text) {
		super(PUow.constructUowReplaceText(text));
	}
	
}
