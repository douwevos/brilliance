package net.natpad.brilliance.eos;

import net.natpad.brilliance.Logger;
import net.natpad.brilliance.LoggerConfig;
import net.natpad.brilliance.LoggerLevel;
import net.natpad.brilliance.chameleon.Cursor;
import net.natpad.brilliance.chameleon.Document;
import net.natpad.brilliance.chameleon.Line;
import net.natpad.brilliance.chameleon.Revision;
import net.natpad.brilliance.chameleon.uow.Uow;
import net.natpad.brilliance.chameleon.uow.UowInsertText;
import net.natpad.brilliance.dragonfly.ConnectorMap;
import net.natpad.brilliance.dragonfly.DragonflyEditor;
import net.natpad.brilliance.dragonfly.IConnectorRequestFactory;
import net.natpad.brilliance.eos.Main.JsonTokenizedLine;
import net.natpad.json.JsonToken;
import net.natpad.json.JsonTokenType;

@LoggerConfig(LoggerLevel.DEBUG)
public class Editor extends DragonflyEditor implements Logger {

	public Editor(Document document, ConnectorMap connectorMap, IConnectorRequestFactory connectorFactory) {
		super(document, connectorMap, connectorFactory);
	}

	
	@Override
	public Uow createInsertOrReplaceUow(int edit_mode, String text) {
		if (edit_mode!=2) {
			return super.createInsertOrReplaceUow(edit_mode, text);
		}
		Revision revision = getDocument().getCurrentRevision();
		Cursor cursor = revision.getCursor();
		Line line = revision.lineAtLocation(cursor.getLineLocation());
		String lineText = line.getText();
		int byteOffset = cursor.getByteOffset();
		int column = Utf8.utf8offset(lineText, byteOffset);
		
		if (text.length()==1) {
			char ch = text.charAt(0);
			log("ch="+(int) ch+", lineText='"+lineText+"'");
			if (ch == '{') {
				JsonTokenizedLine tokenizedLine = new JsonTokenizedLine(lineText);
				boolean allowComplete = true;
				int tokenIdx = tokenizedLine.indexOf(byteOffset);
				if (tokenIdx>=0) {
					JsonToken token = tokenizedLine.get(tokenIdx);
					if (token.getType()==JsonTokenType.QUOTED_STRING) {
						allowComplete = false;
					}
				}
				if (allowComplete) {
					String indent = Utf8.grepIndent(lineText);
					String ft = "{\n"+indent+"  \n"+indent+"}";
					UowInsertText uowInsertText = new UowInsertText(ft, 1, indent.length()+2);
					return uowInsertText;
				}
			} else if (ch=='"') {
				if (column<lineText.length() && lineText.charAt(column)=='"') {
					return new UowInsertText("", 0, 1);
				}
				return new UowInsertText("\"\"", 0, 1);
			} else if ((ch==0xd) || (ch==0xa)) {
				log("did hit enter");
				String indent = Utf8.grepIndent(lineText);
				String trimmedCurrent = lineText.substring(0, column).trim();
				char lastCharBeforeEnter = trimmedCurrent.isEmpty() ? 0 : trimmedCurrent.charAt(trimmedCurrent.length()-1);
				if (lastCharBeforeEnter=='[' || lastCharBeforeEnter=='{') {
					indent = indent + "  ";
				}
				UowInsertText uowInsertText = new UowInsertText("\n"+indent);
				return uowInsertText;
			}
		}
		
		log("################# createInsertOrReplaceUow: "+text.length());
		return super.createInsertOrReplaceUow(edit_mode, text);
	}
	
}
