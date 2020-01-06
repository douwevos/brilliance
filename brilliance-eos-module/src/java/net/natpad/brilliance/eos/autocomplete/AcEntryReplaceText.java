package net.natpad.brilliance.eos.autocomplete;

import net.natpad.brilliance.chameleon.ChameleonEditor;
import net.natpad.brilliance.chameleon.Cursor;
import net.natpad.brilliance.chameleon.DocumentView;
import net.natpad.brilliance.dragonfly.AcContext;
import net.natpad.brilliance.dragonfly.AcEntry;

public class AcEntryReplaceText extends AcEntry {

	private final String textToInsert;
	
	public AcEntryReplaceText(String text) {
		super(text);
		this.textToInsert = text;
	}
	
	public AcEntryReplaceText(String text, String textToInsert) {
		super(text);
		this.textToInsert = textToInsert;
	}
	
	public AcEntryReplaceText(String text, String typeText, boolean enabled, String textToInsert) {
		super(text, typeText, enabled ? 8 : 0, 7, enabled);
		this.textToInsert = textToInsert;
	}

	
	
	@Override
	public void run(AcContext context) {
		ChameleonEditor editor = context.getEditor();
		Cursor startCursor = context.getStartCursor();
		Cursor filterCursor = context.getFilterCursor();
		DocumentView documentView = editor.getDocumentView();
		documentView.setPlainSelection(startCursor, filterCursor);
		editor.postUowInsertText(textToInsert);
	}		
}