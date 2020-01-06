package net.natpad.brilliance.dragonfly;

import net.natpad.brilliance.chameleon.Document;
import net.natpad.brilliance.dragonfly.prime.PFormatProgram;
import net.natpad.brilliance.glibc.GObject;

public class FormatProgram extends GObject {

	public FormatProgram() {
		super(PFormatProgram.create());
		PFormatProgram.construct(this);
	}

	public void add(FormatAction action) {
		PFormatProgram.add(this, action);
	}
	
	public void apply(Document document) {
		PFormatProgram.apply(this, document);
	}
	
}
