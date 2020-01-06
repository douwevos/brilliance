package net.natpad.brilliance.eos.autocomplete;

import net.natpad.brilliance.chameleon.ChameleonEditor;
import net.natpad.brilliance.chameleon.Cursor;
import net.natpad.brilliance.chameleon.DocumentView;
import net.natpad.brilliance.dragonfly.AcContext;
import net.natpad.brilliance.dragonfly.AcEntry;
import net.natpad.brilliance.dragonfly.DragonflyEditor;

public class AcEntryInsertRawTemplate extends AcEntry {

	private final String formText;
	
	public AcEntryInsertRawTemplate(String text, String formText) {
		super(text);
		this.formText = formText;
	}
	

	@Override
	public void run(AcContext context) {
		ChameleonEditor editor = context.getEditor();
		Cursor startCursor = context.getStartCursor();
		Cursor filterCursor = context.getFilterCursor();
		DocumentView documentView = editor.getDocumentView();
		documentView.setPlainSelection(startCursor, filterCursor);
		DragonflyEditor ded = (DragonflyEditor) editor;
		ded.runUowInsertTemplateRaw(formText);
	}		
}