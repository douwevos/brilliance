package net.natpad.brilliance.eos;

import java.io.IOException;
import java.io.StringReader;
import java.util.List;
import java.util.Objects;

import com.tickeos.core.service.impl.module.dto.ArticleDto;
import com.tickeos.core.service.impl.module.dto.DynamicProductPropertyGroupDto;
import com.tickeos.core.service.impl.module.dto.ModuleDto;
import com.tickeos.core.service.impl.module.dto.TransportProductDto;

import net.natpad.brilliance.Logger;
import net.natpad.brilliance.LoggerConfig;
import net.natpad.brilliance.LoggerLevel;
import net.natpad.brilliance.chameleon.Cursor;
import net.natpad.brilliance.chameleon.CursorM;
import net.natpad.brilliance.chameleon.Document;
import net.natpad.brilliance.chameleon.Revision;
import net.natpad.brilliance.chameleon.RevisionReader;
import net.natpad.brilliance.dragonfly.DragonflyEditor;
import net.natpad.brilliance.dragonfly.EditorPanel;
import net.natpad.brilliance.eos.autocomplete.AutoCompleteModel;
import net.natpad.json.JsonArray;
import net.natpad.json.JsonField;
import net.natpad.json.JsonNode;
import net.natpad.json.JsonNodeAccess;
import net.natpad.json.JsonObject;
import net.natpad.json.JsonToken;
import net.natpad.json.JsonTokenType;
import net.natpad.json.JsonScanner.Location;
import net.natpad.json.mapper.JsonMapper;

@LoggerConfig(LoggerLevel.DEBUG)
public class OpenDeclarationHandler implements Logger {

	private final EditorPanel editorPanel;
	
	public OpenDeclarationHandler(EditorPanel editorPanel) {
		this.editorPanel = editorPanel;
	}

	public void openAt(Cursor cursor) {
		log("should open declaration at:" + cursor);
		if (cursor!=null) {
			DragonflyEditor editor = editorPanel.getEditor();
			Document document = editor.getDocument();
			Revision revision = document.getCurrentRevision();
			RevisionReader reader = new RevisionReader(revision);
			String fullText = reader.readFullText();
////			log("fullText"+fullText.length());
////
//			
			CursorM cursorM = revision.toCursorM(cursor);
			long row = cursorM.getRow();
			int columnu8 = cursorM.getColumn();
//			
			try {
				JsonMapper jsonMapper = new JsonMapper(new StringReader(fullText));
				ModuleDto moduleDto = jsonMapper.map(ModuleDto.class);
				
				
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
					JsonNode parentNode = tokenLocked.getParentNode();
					log("tokenLocked="+tokenLocked);
					log("parentNode="+parentNode);
					if (parentNode!= null) {
						JsonNode parentNode2 = parentNode.getParentNode();
						log("parentNode2="+parentNode2);
						if (parentNode2!=null) {
							if (parentNode2 instanceof JsonArray) {
								JsonNode parentNode3 = parentNode2.getParentNode();
								log("parentNode3="+parentNode3);
								if (parentNode3 instanceof JsonField) {
									JsonField field = (JsonField) parentNode3;
									log("field="+field.name());
									if ("propertyGroups".equals(field.name())) {
										// TODO unsafe cast
										String groupName = (String) tokenLocked.getValue();
										log("should open declaration of "+groupName);
										DynamicProductPropertyGroupDto dynamicProductPropertyGroupDto = moduleDto.getDynamicProductPropertyGroups().stream().filter(s -> Objects.equals(groupName, s.getSymbolicName())).findAny().orElse(null);
										
										JsonToken destinationToken = tokenList.stream().filter(t -> new JsonNodeAccess(t).parent().parent().parent().output() == dynamicProductPropertyGroupDto).findAny().orElse(null);
										log("destinationToken "+destinationToken);
										if (destinationToken != null) {
											log("now jumping to line:"+destinationToken.getLeft().getRow());
											boolean gotoLine = editor.gotoLine(destinationToken.getLeft().getRow());											
											log("gotoLine="+gotoLine);
										}
									} else if ("products".equals(field.name())) {
										openProductBySymbolicName(tokenLocked, moduleDto, jsonMapper);
									}
								}
							} else if (parentNode2 instanceof JsonField) {
								JsonField fieldNode = (JsonField) parentNode2;
								if ("symbolicName".equals(fieldNode.name())) {
									JsonObject objectNodePropValue = (JsonObject) fieldNode.getParentNode();
									JsonArray arrayNodePropValues = (JsonArray) objectNodePropValue.getParentNode();
									JsonField fieldPropValues = (JsonField) arrayNodePropValues.getParentNode();
									if ("propertyValues".contentEquals(fieldPropValues.name())) {
										Object output = fieldPropValues.getParentNode().getOutput();
										log("output="+output);
										
										AutoCompleteModel model = null;
										
										if (output instanceof TransportProductDto) {
											TransportProductDto product = (TransportProductDto) output;
											model = new AutoCompleteModel(jsonMapper, moduleDto);
											model.buildForTransportProduct(product);
										} 
										else if (output instanceof ArticleDto) {
											ArticleDto aritcle = (ArticleDto) output;
											model = new AutoCompleteModel(jsonMapper, moduleDto);
											model.buildForArticle(aritcle);
										}
										
										String symbolicName = (String) tokenLocked.getValue();
										Object declaratedBy = model.findPropertyTypeDeclaration(symbolicName);
										log("declaratedBy="+declaratedBy);
										if (declaratedBy != null) {
											JsonToken destinationToken = tokenList.stream().filter(t -> t.getOutput() == declaratedBy).findAny().orElse(null);
											if (destinationToken != null) {
												log("now jumping to line:"+destinationToken.getLeft().getRow());
												boolean gotoLine = editor.gotoLine(destinationToken.getLeft().getRow());											
												log("gotoLine="+gotoLine);
											}
											
										}
									}
									
								}
							}

						}
					}
				}
				
			} catch (IOException e) {
				e.printStackTrace();
			}			
		}		
	}

	private void openProductBySymbolicName(JsonToken expectQuotedStringToken, ModuleDto moduleDto, JsonMapper jsonMapper) {
		if (expectQuotedStringToken.getType()!=JsonTokenType.QUOTED_STRING) {
			log("expected quoted string: "+expectQuotedStringToken);
			return;
		}
		String productName = (String) expectQuotedStringToken.getValue();
		TransportProductDto productDto = moduleDto.getTransportProducts().stream().filter(tp -> Objects.equals(tp.getSymbolicName(), productName)).findAny().orElse(null);
		if (productDto == null) {
			log("no such product found: "+productName);
			return;
		}
		
		List<JsonToken> tokenList = jsonMapper.getTokenList();
		JsonToken destinationToken = tokenList.stream().filter(t -> t.getOutput() == productDto).findAny().orElse(null);
		if (destinationToken != null) {
			log("now jumping to line:"+destinationToken.getLeft().getRow());
			DragonflyEditor editor = editorPanel.getEditor();
			boolean gotoLine = editor.gotoLine(destinationToken.getLeft().getRow());											
			log("gotoLine="+gotoLine);
		}
		
	}

}
