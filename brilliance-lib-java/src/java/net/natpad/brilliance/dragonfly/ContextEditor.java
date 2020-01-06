package net.natpad.brilliance.dragonfly;

import net.natpad.brilliance.chameleon.Cursor;
import net.natpad.brilliance.glibc.GObject;

public abstract class ContextEditor extends GObject {

	public ContextEditor(EditorPanel panel) {
		super(0);
		id = Dragonfly.createContextEditor(this, panel);
	}
	
	
	public abstract ControlLeftRight calculateControlLeftRight(Cursor cursor);
	
	public abstract void openDeclaration(Cursor cursor);
	
	public abstract void format();
	
	public abstract void contextInsertText(String text);
}
