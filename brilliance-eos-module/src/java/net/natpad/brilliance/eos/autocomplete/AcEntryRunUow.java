package net.natpad.brilliance.eos.autocomplete;

import net.natpad.brilliance.chameleon.uow.Uow;
import net.natpad.brilliance.dragonfly.AcContext;
import net.natpad.brilliance.dragonfly.AcEntry;

public class AcEntryRunUow extends AcEntry {

	private final Uow uow;
	
	public AcEntryRunUow(String text, Uow uow) {
		super(text);
		this.uow = uow;
	}
	
	@Override
	public void run(AcContext context) {
		context.getEditor().postUow(uow);
	}
}
