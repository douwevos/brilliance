package net.natpad.brilliance.eos;

import java.io.IOException;
import java.io.StringReader;
import java.util.ArrayList;
import java.util.List;

import com.tickeos.core.service.impl.module.dto.ModuleDto;

import net.natpad.brilliance.Logger;
import net.natpad.brilliance.chameleon.Cursor;
import net.natpad.brilliance.chameleon.CursorM;
import net.natpad.brilliance.chameleon.Document;
import net.natpad.brilliance.chameleon.Line;
import net.natpad.brilliance.chameleon.Revision;
import net.natpad.brilliance.chameleon.RevisionReader;
import net.natpad.brilliance.dragonfly.AcContext;
import net.natpad.brilliance.dragonfly.ContextEditor;
import net.natpad.brilliance.dragonfly.ControlLeftRight;
import net.natpad.brilliance.dragonfly.EditorPanel;
import net.natpad.brilliance.dragonfly.FormatAction;
import net.natpad.brilliance.dragonfly.FormatProgram;
import net.natpad.brilliance.dragonfly.IAcContentProvider;
import net.natpad.brilliance.eos.autocomplete.AutoCompleteHandler;
import net.natpad.json.JsonScanner;
import net.natpad.json.JsonScanner.Location;
import net.natpad.json.JsonToken;
import net.natpad.json.JsonTokenType;
import net.natpad.json.mapper.JsonMapper;

public class EosContextEditor extends ContextEditor implements IAcContentProvider, Logger {

	private static final boolean FORCE_LINE_BREAK_AFTER_COMMA = false;

	private static final int TAB_SIZE = 2;

	// TODO use/create getEditorPanel to fix reference problems;
	private EditorPanel editorPanel;
	
	private final AutoCompleteHandler autoCompleteHandler = new AutoCompleteHandler();
	
	public EosContextEditor(EditorPanel editorPanel) {
		super(editorPanel);
		this.editorPanel = editorPanel;
	}
	
	@Override
	public void contextInsertText(String text) {
		Document document = editorPanel.getEditor().getDocument();
		Revision revision = document.getCurrentRevision();
		Cursor cursor = revision.getCursor();
		Line line = revision.lineAtLocation(cursor.getLineLocation());
		String textLine = line.getText();
		int index = Utf8.utf8offset(textLine, cursor.getByteOffset());
		if ("{".equals(text)) {
			log("textLine="+textLine);
			log("index="+index);
		}
		
		editorPanel.getEditor().postUowInsertText(text);
		
	}
	
	@Override
	public void format() {
		Document document = editorPanel.getEditor().getDocument();
		Revision revision = document.getCurrentRevision();
		RevisionReader reader = new RevisionReader(revision);
		String fullText = reader.readFullText();
		try {
			JsonScanner scanner = new JsonScanner(new StringReader(fullText));
			
			int indent = 0;
			int out_row = 0;
			int out_column = 0;
			int out_offset = 0;

			boolean done = false;

			List<JsonToken> token_in = new ArrayList<JsonToken>();
			List<JsonToken> token_out = new ArrayList<JsonToken>();

			JsonToken last_token = null;

			while(!done) {
				JsonToken token = scanner.next();
				log("token="+token);
				JsonToken out = null;
				switch(token.getType()) {
					case LEFT_BRACE :
					case LEFT_BRACKET : {
						if (!FORCE_LINE_BREAK_AFTER_COMMA && last_token!=null && last_token.getType()==JsonTokenType.COMMA) {
							out_column = last_token.getRight().getColumn()+1;
							out_offset = last_token.getRight().getUtf8offset()+1;
							out_row = last_token.getRight().getRow();
						}
						out = new JsonToken(token.getType(), new Location(out_column, out_row, out_offset), new Location(out_column+1, out_row, out_offset+1));
						out_row++;
						indent++;
						out_column = indent * TAB_SIZE;
						out_offset = out_column;
					} break;
					case RIGHT_BRACE :
					case RIGHT_BRACKET : {
						out_row++;
						indent--;
						out_column = indent * TAB_SIZE;
						out_offset = out_column;
						out = new JsonToken(token.getType(), new Location(out_column, out_row, out_offset), new Location(out_column+1, out_row, out_offset+1));
						out_column++;
						out_offset++;
					} break;


//					case JOR_SYMBOL_INVALID :
					case ATTRIBUTE_NAME :
					case NULL :
					case TRUE :
					case FALSE :
					case NUMBER :
//					case JOR_SYMBOL_KEY :
					case QUOTED_STRING : {
						int start_column = out_column;
						int start_offset = out_offset;
						out_column += token.getRight().getColumn()-token.getLeft().getColumn();
						out_offset += token.getRight().getUtf8offset()-token.getLeft().getUtf8offset();
						out = new JsonToken(token.getType(), new Location(start_column, out_row, start_offset), new Location(out_column, out_row, out_offset));
					} break;

					case COMMA : {
						out = new JsonToken(token.getType(), new Location(out_column, out_row, out_offset), new Location(out_column+1, out_row, out_offset+1));
						out_column += 2;
						out_column = indent * TAB_SIZE;
						out_offset = out_column;
						out_row++;
					} break;

					case COLON : {
						out = new JsonToken(token.getType(), new Location(out_column+1, out_row, out_offset+1), new Location(out_column+2, out_row, out_offset+2));
						out_column += 3;
						out_offset += 3;
					} break;

					case EOT :
						done = true;
						break;
				}

				if (out != null) {
					token_in.add(token);
					token_out.add(out);
					last_token = out;
				}
			}			

			
			FormatProgram formatProgram = new FormatProgram();

			JsonToken mtoken_out_ahead = null;
			JsonToken mtoken_in_ahead = null;
			for(int idx=token_out.size()-1; idx>=0; idx--) {
				JsonToken mtoken_out = token_out.get(idx);
				JsonToken mtoken_in = token_in.get(idx);

				log("idx="+idx+", in="+mtoken_in+", out="+mtoken_out);

				if (mtoken_out==null) {
					continue;
				}


				if (mtoken_out_ahead!=null) {

					Location in_left = mtoken_in.getRight();
					Location in_right = mtoken_in_ahead.getLeft();
					Location out_left = mtoken_out.getRight();
					Location out_right = mtoken_out_ahead.getLeft();
					
//					if (idx<30 ) {
						FormatAction action = new FormatAction(in_left.getUtf8offset(), in_left.getRow(), in_right.getUtf8offset(), in_right.getRow(), out_left.getUtf8offset(), out_left.getRow(), out_right.getUtf8offset(), out_right.getRow());
						formatProgram.add(action);
//					}
//					
//					
//					ChaCursorMWo *sOne = cha_cursorm_wo_new(mtoken_in->end_row, mtoken_in->end_offset);
//					ChaCursorMWo *sTwo = cha_cursorm_wo_new(mtoken_in_ahead->start_row, mtoken_in_ahead->start_offset);
//
//					log("removing between %O and %O, line-count=%ld", sOne, sTwo, cha_revision_wo_count_lines(e_revision));
//
//
//					cha_revision_wo_remove_between_cursors(e_revision, sOne, sTwo, TRUE, null);
//
//					log("finished remove");
//
//					CatArrayWo *lines_to_insert = cat_array_wo_new();
//					long long ro = mtoken_out->end_row;
//					while(ro<mtoken_out_ahead->start_row) {
//						ChaLineWo *empty_line = cha_line_wo_new_anchored(cat_ref_ptr(empty_str), line_end);
//						cat_array_wo_append(lines_to_insert, (GObject *) empty_line);
//						ro++;
//						cat_unref_ptr(empty_line);
//					}
//
//					int space_count = 0;
//					if (mtoken_out->end_row==mtoken_out_ahead->start_row) {
//						space_count = mtoken_out_ahead->start_column - mtoken_out->end_column;
//					} else {
//						space_count = mtoken_out_ahead->start_column;
//					}
//
//					CatStringWo *sl = cat_string_wo_new();
//					while(space_count>0) {
//						cat_log_trace("space_count=%d", space_count);
//						if (space_count>=4) {
//							cat_string_wo_append_chars_len(sl, "    ", 4);
//							space_count-=4;
//						} else {
//							cat_string_wo_append_chars_len(sl, "    ", space_count);
//							space_count = 0;
//						}
//					}
//					ChaLineWo *sl_line = cha_line_wo_new_anchored(sl, CHA_LINE_END_NONE);
//					cat_array_wo_append(lines_to_insert, (GObject *) sl_line);
//					cat_unref_ptr(sl_line);
//
//					cha_revision_wo_insert_lines(e_revision, sOne, lines_to_insert, TRUE);
//					cat_unref_ptr(lines_to_insert);
//					cat_unref_ptr(sOne);
//					cat_unref_ptr(sTwo);
				}

				mtoken_out_ahead = mtoken_out;
				mtoken_in_ahead = mtoken_in;
			}
				
			formatProgram.apply(document);

			
			
		} catch (IOException e) {
			e.printStackTrace();
		}
		
		
	}
	
	@Override
	public ControlLeftRight calculateControlLeftRight(Cursor cursor) {
		ControlLeftRight result = null;
		if (cursor!=null) {
			Document document = editorPanel.getEditor().getDocument();
			Revision revision = document.getCurrentRevision();
			RevisionReader reader = new RevisionReader(revision);
			String fullText = reader.readFullText();
////			log("fullText"+fullText.length());
////
//			
			CursorM cursorM = revision.toCursorM(cursor);
//
			long row = cursorM.getRow();
			int columnu8 = cursorM.getColumn();
//			
			try {
				JsonMapper jsonMapper = new JsonMapper(new StringReader(fullText));
				ModuleDto test = jsonMapper.map(ModuleDto.class);
				
				
				List<JsonToken> tokenList = jsonMapper.getTokenList();
//				JsonScanner jsonScanner = new JsonScanner(new StringReader(fullText));
//				JsonParser jsonParser = new JsonParser(jsonScanner);
//				JsonValue value = jsonParser.parse();
//				List<JsonToken> tokenList = jsonParser.getTokenList();
				JsonToken tokenLocked = null;
				for(JsonToken token : tokenList) {
					Location left = token.getLeft();
					Location right = token.getRight();
					if (row>=left.getRow() && row<=right.getRow()) {
						
						if (columnu8>=left.getUtf8offset() && columnu8<=right.getUtf8offset()) {
							tokenLocked = token;
							break;
						}
					}
				}
				log("tokenLocked="+tokenLocked);
				
				if (tokenLocked != null) {
					int left = tokenLocked.getLeft().getUtf8offset();
					int right = tokenLocked.getRight().getUtf8offset();
					result = new ControlLeftRight(left, right);
				}
				
			} catch (IOException e) {
				e.printStackTrace();
			}			
		}
//		result = new ControlLeftRight(cursor.getByteOffset(), cursor.getByteOffset()+4);
		return result;
	}
	
	
	@Override
	public void openDeclaration(Cursor cursor) {
		OpenDeclarationHandler handler = new OpenDeclarationHandler(editorPanel);
		handler.openAt(cursor);

	}
	
	
	@Override
	public void enlistContent(AcContext context) {
		RevisionReader reader = context.createRevisionReader();
		String fullText = reader.readFullText();
//		log("got fullText");
		
		Cursor endCursor = context.getEndCursor();
		Revision revision = context.getRevision();
		
		CursorM cursorM = revision.toCursorM(endCursor);
//		log("cursorM="+cursorM);
		
//		log("-- filter:  "+context.getFilterCursor());
//		log("-- start :  "+context.getStartCursor());
//		log("-- end   :  "+context.getEndCursor());
		
		try {
			JsonMapper jsonMapper = new JsonMapper(new StringReader(fullText));
			ModuleDto moduleDto = jsonMapper.map(ModuleDto.class);
			
			autoCompleteHandler.enlist(context, cursorM, jsonMapper, moduleDto);
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	
}
