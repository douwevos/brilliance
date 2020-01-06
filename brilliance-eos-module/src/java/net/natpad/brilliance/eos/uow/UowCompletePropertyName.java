package net.natpad.brilliance.eos.uow;

import java.io.IOException;
import java.io.StringReader;
import java.util.List;

import com.tickeos.core.service.impl.module.dto.ModuleDto;

import net.natpad.brilliance.chameleon.ChameleonEditor;
import net.natpad.brilliance.chameleon.Cursor;
import net.natpad.brilliance.chameleon.CursorM;
import net.natpad.brilliance.chameleon.Document;
import net.natpad.brilliance.chameleon.DocumentView;
import net.natpad.brilliance.chameleon.Line;
import net.natpad.brilliance.chameleon.LineLocation;
import net.natpad.brilliance.chameleon.Revision;
import net.natpad.brilliance.chameleon.RevisionReader;
import net.natpad.brilliance.chameleon.uow.UowRemote;
import net.natpad.brilliance.eos.Utf8;
import net.natpad.json.JsonToken;
import net.natpad.json.JsonTokenType;
import net.natpad.json.JsonField;
import net.natpad.json.JsonNode;
import net.natpad.json.JsonScanner.Location;
import net.natpad.json.mapper.EnumTypeMapper;
import net.natpad.json.mapper.JsonMapper;
import net.natpad.json.mapper.ListTypeMapper;
import net.natpad.json.mapper.ObjectTypeMapper;
import net.natpad.json.mapper.TypeMapper;
import net.natpad.json.mapper.TypeMapperString;

public class UowCompletePropertyName extends UowRemote {

	private final String propertyName;
	
	public UowCompletePropertyName(String propertyName) {
		super(false);
		this.propertyName = propertyName;
	}
	
	@Override
	public void run(ChameleonEditor editor, DocumentView documentView) {
		Document document = editor.getDocument();
		boolean isEditable = document.isEditable();
		Revision editableRevision = document.getEditableRevision();
		RevisionReader revisionReader = new RevisionReader(editableRevision);
		String fullText = revisionReader.readFullText();
		JsonMapper jsonMapper;
		try {
			jsonMapper = new JsonMapper(new StringReader(fullText));
			ModuleDto moduleDto = jsonMapper.map(ModuleDto.class);

			Cursor cursor = editableRevision.getCursor();
			CursorM cursorM = editableRevision.toCursorM(cursor);
			long row = cursorM.getRow();
			int columnu8 = cursorM.getColumn();
			List<JsonToken> tokenList = jsonMapper.getTokenList();
			JsonToken tokenLocked = null;
			int tokenIndex = 0;
			for(JsonToken token : tokenList) {
				Location left = token.getLeft();
				Location right = token.getRight();
				if (row>=left.getRow() && row<=right.getRow()) {
					
					if (columnu8>=left.getUtf8offset() && columnu8<=right.getUtf8offset()) {
						tokenLocked = token;
						break;
					}
				}
				tokenIndex++;
			}
			log("tokenLocked="+tokenLocked);

			TypeMapper fieldValueMapper = null;
			
			if (tokenLocked!=null) {
				log("tokenLocked.output="+tokenLocked.getOutput());
				log("tokenLocked.parent="+tokenLocked.getParentNode());
				if (tokenLocked.getParentNode()!=null) {
					if (tokenLocked.getParentNode() instanceof JsonField) {
						JsonField field = (JsonField) tokenLocked.getParentNode();
						if (field.getParentNode()!=null) {
							Object output = field.getParentNode().getOutput();
							if (output!=null) {
								TypeMapper<?> mapper = jsonMapper.getMapper(output.getClass());
								if (mapper instanceof ObjectTypeMapper) {
									ObjectTypeMapper omapper = (ObjectTypeMapper) mapper;
									fieldValueMapper = omapper.getFieldValueMapper(propertyName);
								}
							}
						}
					}
					log("tokenLocked.parent.out="+tokenLocked.getParentNode().getOutput());
					JsonNode parentNode = tokenLocked.getParentNode().getParentNode();
					log("parent="+parentNode);
					if (parentNode!=null) {
						log("parent.out="+parentNode.getOutput());
					}
				}
				
				
				Location left = tokenLocked.getLeft();
				Location right = tokenLocked.getRight();
				
				
				
				Cursor startCursor = new Cursor(cursor.getLineLocation(), left.getUtf8offset(), 0);
				Cursor filterCursor = new Cursor(cursor.getLineLocation(), right.getUtf8offset(), 0);
				documentView.setPlainSelection(startCursor, filterCursor);
				documentView.removeSelection();

				int deltaColumn = 0;
				int deltaRow = 0;
				String text = "\"" + propertyName + "\"";
				if (tokenIndex+1<tokenList.size()) {
					JsonToken nextJsonToken = tokenList.get(tokenIndex+1);
					if (tokenLocked.getType() == JsonTokenType.ATTRIBUTE_NAME && nextJsonToken.getType()!=JsonTokenType.COLON) {
						text += " : ";
						
						if (fieldValueMapper != null) {
							if (fieldValueMapper instanceof ObjectTypeMapper) {
								Line lineAtLocation = editableRevision.lineAtLocation(cursor.getLineLocation());
								String lineText = lineAtLocation.getText();
								String indent = Utf8.grepIndent(lineText);
								text += "{\n"+indent+"  \n"+indent+"}";
								deltaRow = -1;
								deltaColumn = indent.length()+2;
							} else if (fieldValueMapper instanceof ListTypeMapper) {
								Line lineAtLocation = editableRevision.lineAtLocation(cursor.getLineLocation());
								text += "[ ]";
								deltaColumn = -1;
							} else if ((fieldValueMapper instanceof TypeMapperString) || (fieldValueMapper instanceof EnumTypeMapper)) {
								text += "\"\"";
								deltaColumn = -1;
							}

						}
						
					}
				}

				
				editableRevision.insertAsLines(text);
				
				if (deltaRow!=0) {
					Cursor cursorOut = editableRevision.getCursor();
					long rowOut = editableRevision.calculateRow(cursorOut.getLineLocation());
					rowOut = rowOut+deltaRow;
					LineLocation lineLocation = editableRevision.calculateLineLocation(rowOut);
					editableRevision.setCursor(new Cursor(lineLocation, deltaColumn, 0));
				}
				else if (deltaColumn!=0) {
					Cursor cursorOut = editableRevision.getCursor();
					editableRevision.setCursor(new Cursor(cursorOut.getLineLocation(), cursorOut.getByteOffset()+deltaColumn, 0));
				}
				
			}
			
		} catch (IOException e) {
			e.printStackTrace();
		}
		
		if (!isEditable) {
			document.anchorDocument();
			documentView.moveViewToFocus(false);
		}
		
	}
	
}
