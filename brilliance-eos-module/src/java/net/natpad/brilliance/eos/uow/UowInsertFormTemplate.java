package net.natpad.brilliance.eos.uow;

import net.natpad.brilliance.chameleon.ChameleonEditor;
import net.natpad.brilliance.chameleon.Cursor;
import net.natpad.brilliance.chameleon.Document;
import net.natpad.brilliance.chameleon.DocumentView;
import net.natpad.brilliance.chameleon.Line;
import net.natpad.brilliance.chameleon.LineLocation;
import net.natpad.brilliance.chameleon.Revision;
import net.natpad.brilliance.chameleon.uow.UowRemote;
import net.natpad.brilliance.dragonfly.DragonflyEditor;
import net.natpad.brilliance.eos.Utf8;

public class UowInsertFormTemplate extends UowRemote {

	String form;
	int tabSize = 2;
	
	public UowInsertFormTemplate(String form) {
		super(false);
		this.form = form;
	}
	
	@Override
	public void run(ChameleonEditor editor, DocumentView documentView) {
		Document document = editor.getDocument();
		boolean isEditable = document.isEditable();
//		documentView.removeSelection();
		System.out.println("removed selection");
		Revision editableRevision = document.getEditableRevision();
		Cursor cursor = editableRevision.getCursor();
		System.out.println("got cursor:"+cursor);
		LineLocation lineLocation = cursor.getLineLocation();
		System.out.println("got lineLocation:"+lineLocation);
		long row = editableRevision.calculateRow(lineLocation);
		System.out.println("row="+row);
		
		
		Line currentLine = editableRevision.lineAtLocation(lineLocation);
		String text = currentLine.getText();
		String grepIndent = null;
//		if (text.trim().isEmpty()) {
//			/* Current line is empty. lets see what the line before does. */
//			if (row>0) {
//				LineLocation precLineLocation = editableRevision.calculateLineLocation(row-1);
//				Line precLine = editableRevision.lineAtLocation(precLineLocation);
//				String precText = precLine.getText();
//				String trim = precText.trim();
//				if (!precText.trim().isEmpty()) {
//					grepIndent = Utf8.grepIndent(precText);
//					if (trim.endsWith("{") || trim.endsWith("[")) {
//						grepIndent += "  ";
//					} else if (trim.endsWith("}") || trim.endsWith("]")) {
//						
//					}
//				}
//			}
//		}
		
		String endForm = form;
		
		int column = Utf8.utf8offset(text, cursor.getByteOffset());
		String restEnd = text.substring(column).trim();
		boolean needLfAtStart = false;
		boolean needLfAtEnd = false;
		Character precChar = null;
		if (!restEnd.isEmpty()) {
			needLfAtEnd = true;
			precChar = restEnd.charAt(0);
		} else {
			LineLocation lineLocationAfter = editableRevision.calculateLineLocation(row+1);
			if (lineLocationAfter!=null) {
				Line lineAfter = editableRevision.pageAt(lineLocationAfter.getPageIndex()).lineAt(lineLocationAfter.getPageLineIndex());
				String trimmedAfter = lineAfter.getText().trim();
				if (!trimmedAfter.isEmpty()) {
					precChar = trimmedAfter.charAt(0);
				}
			}
		}

		if (precChar!=null) {
			if (precChar=='{' || precChar=='[') {
				endForm += ',';
			}
		}
		if (needLfAtEnd) {
			endForm += '\n';
		}
		
		String left = text.substring(0, column);
		grepIndent = Utf8.grepIndent(left);
		System.out.println("column="+column+", grepIndent='"+grepIndent+"', text='"+text+"', left='"+left+"'");
		
		String[] split = endForm.split("\n");
		StringBuilder newForm = new StringBuilder();
		for(int idx=0; idx<split.length; idx++) {
			if (idx==0) {
				split[idx] = autoIndentLine(split[idx], "");	
			} else {
				newForm.append("\n");
				split[idx] = autoIndentLine(split[idx], grepIndent);				
			}
			newForm.append(split[idx]);
		}
		
		System.out.println(""+newForm.toString());
		
		DragonflyEditor ded = (DragonflyEditor) editor;
		ded.runUowInsertTemplateRaw(newForm.toString());

		if (!isEditable) {
			document.anchorDocument();
			documentView.moveViewToFocus(false);
		}

		
	}

	private String autoIndentLine(String input, String baseIndent) {
		StringBuilder buf = new StringBuilder(baseIndent);
		String singleTab = "  ";
		for(int idx=0; idx<input.length(); idx++) {
			char ch = input.charAt(idx);
			if (ch==' ' || ch=='\t') {
				buf.append(singleTab);
			} else {
				buf.append(input.substring(idx));
				return buf.toString();
			}
		}
		return "";
	}
}
