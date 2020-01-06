package net.natpad.brilliance.dragonfly;

import net.natpad.brilliance.glibc.GObject;

public abstract class AcEntry extends GObject {

	private final String text;
	
	public AcEntry(String text) {
		super(Dragonfly.createAcEntry(text));
		Dragonfly.acEntryLink(this);
		this.text = text;
	}

	public AcEntry(String text, String typeText, int colorCode, int typeColorCode, boolean enabled) {
		super(Dragonfly.constructAcEntryFull(text, typeText, colorCode, typeColorCode, enabled));
		Dragonfly.acEntryLink(this);
		this.text = text;
	}

	@Override
	protected void finalize() throws Throwable {
		Dragonfly.acEntryUnlink(this);
		super.finalize();
	}

	
	public abstract void run(AcContext context);
	
	public String getText() {
		return text;
	}
}
