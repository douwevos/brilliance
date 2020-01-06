package net.natpad.brilliance.eos.autocomplete;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.stream.Collectors;

import org.fpel.Fpel;
import org.fpel.FpelEvaluable;
import org.fpel.FpelExpression;
import org.fpel.FpelNode;
import org.fpel.FpelRawOuput;
import org.fpel.evaluable.FpelArrayAccess;
import org.fpel.evaluable.FpelGetProperty;

import com.tickeos.core.service.impl.module.dto.ArticleDto;
import com.tickeos.core.service.impl.module.dto.EnumerationTypeDto;
import com.tickeos.core.service.impl.module.dto.ModuleDto;
import com.tickeos.core.service.impl.module.dto.PropertyTypeDto;
import com.tickeos.core.service.impl.module.dto.PropertyValueDto;
import com.tickeos.core.service.impl.module.dto.TransportProductDto;

import net.natpad.brilliance.Logger;
import net.natpad.brilliance.LoggerConfig;
import net.natpad.brilliance.LoggerLevel;
import net.natpad.brilliance.chameleon.CursorM;
import net.natpad.brilliance.chameleon.uow.Uow;
import net.natpad.brilliance.dragonfly.AcContext;
import net.natpad.brilliance.dragonfly.AcEntry;
import net.natpad.brilliance.eos.Utf8;
import net.natpad.brilliance.eos.autocomplete.AutoCompleteModel.PropertyNameAndSource;
import net.natpad.brilliance.eos.uow.UowCompletePropertyName;
import net.natpad.brilliance.eos.uow.UowInsertFormTemplate;
import net.natpad.json.JsonArray;
import net.natpad.json.JsonField;
import net.natpad.json.JsonNode;
import net.natpad.json.JsonObject;
import net.natpad.json.JsonScanner.Location;
import net.natpad.json.JsonSimpleValue;
import net.natpad.json.JsonToken;
import net.natpad.json.JsonValue;
import net.natpad.json.mapper.EnumTypeMapper;
import net.natpad.json.mapper.JsonMapper;
import net.natpad.json.mapper.ObjectTypeMapper;
import net.natpad.json.mapper.TypeMapper;

@LoggerConfig(LoggerLevel.DEBUG)
public class AutoCompleteHandler implements Logger {
	
	public void enlist(AcContext context, CursorM cursorM, JsonMapper jsonMapper, ModuleDto moduleDto) {

		long row = cursorM.getRow();
		int columnu8 = cursorM.getColumn();

		List<JsonToken> tokenList = jsonMapper.getTokenList();
		
		JsonToken tokenLocked = null;
		JsonToken tokenBefore = null;
		for(JsonToken token : tokenList) {
			Location left = token.getLeft();
			Location right = token.getRight();
			if ((right.getRow()<row) ||  (right.getRow()<row && right.getUtf8offset()<=columnu8)) {
				tokenBefore = token;
			}
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
			log("parentNode = "+parentNode);
			if (parentNode instanceof JsonField) {
				autoCompleteForJsonField(context, jsonMapper, (JsonField) parentNode, tokenLocked);
			} else if (parentNode instanceof JsonSimpleValue) {
				JsonNode superNode = parentNode.getParentNode();
				log("superNode = "+superNode);
				if (superNode instanceof JsonField) {
					autoCompleteForJsonFieldValue(context, jsonMapper, (JsonField) superNode, tokenLocked, columnu8, moduleDto);
				} else if (superNode.getParentNode() instanceof JsonField) {
					JsonField ownerField = (JsonField) superNode.getParentNode();
					if ("propertyGroups".equals(ownerField.name())) {
						autoCompletePropertyGroupsEntry(context, jsonMapper, tokenLocked, moduleDto);
					} else if ("products".equals(ownerField.name())) {
						autoCompleteProducts(context, jsonMapper, tokenLocked, moduleDto);
					}
				}
			}
		}
		
		
		if (tokenLocked == null && tokenBefore!=null) {
			log("tokenBefore="+tokenBefore+" :: parent="+tokenBefore.getParentNode());
			if (tokenBefore.getParentNode() instanceof JsonArray) {
				JsonNode parentNode = tokenBefore.getParentNode().getParentNode();
				if (parentNode instanceof JsonField) {
					JsonField field = (JsonField) parentNode;
					if ("properties".equals(field.name())) {
						autoCompletePropertyTemplates(context, jsonMapper, tokenLocked, moduleDto);
					}
				}
			}
		}
	}

	
	private String[][] data = new String[][] {
		    {"Base text-property", "{\n \"symbolicName\": \"[%name%]\",\n \"displayName\": \"[%name%]\",\n \"source\": \"[%PRODUCT%]\",\n \"type\": \"_Text\"\n}"},
			{"Openshop Beschreibung", "{\n \"symbolicName\": \"beschrbg\",\n \"displayName\": \"Beschreibung\",\n \"source\": \"[%PRODUCT%]\",\n \"type\": \"_TextMultiline\"\n}"},
			{"Openshop Gattung", "{\n \"symbolicName\": \"gattung_db\",\n \"displayName\": \"Gattung (DB)\",\n \"source\": \"[%PRODUCT%]\",\n \"type\": \"_Text\"\n}"},
			{"Openshop Auskunft-ID", "{\n \"symbolicName\": \"id_auskunft\",\n \"displayName\": \"Bezeichnung im Auskunftssystem\",\n \"source\": \"[%PREDEFINED%]\",\n \"type\": \"_Text\"\n}"},
			{"Openshop mot_kond", "{\n \"symbolicName\": \"mot_kond\",\n \"displayName\": \"Konditionen MOT\",\n \"source\": \"[%PRODUCT%]\",\n \"type\": \"_TextMultiline\"\n}"},
	};
	

	private void autoCompletePropertyTemplates(AcContext context, JsonMapper jsonMapper, JsonToken tokenLocked,
			ModuleDto moduleDto) {

		for(String[] t : data) {
			Uow uow = new UowInsertFormTemplate(t[1]);
			AcEntryRunUow entry = new AcEntryRunUow(t[0], uow);
			context.addEntry(entry);
			
		}
		
//		String form = "{\n" + 
//				" \"symbolicName\": \"beschrbg\",\n" + 
//				" \"displayName\": \"Beschreibung\",\n" + 
//				" \"source\": \"[%PRODUCT%]\",\n" + 
//				" \"type\": \"_TextMultiline\"\n" + 
//				"}";
////		AcEntryInsertRawTemplate entry = new AcEntryInsertRawTemplate("Openshop Beschreibung", form );
//		Uow uow = new UowInsertFormTemplate(form);
//		AcEntryRunUow entry = new AcEntryRunUow("Openshop Beschreibung", uow);
//		context.addEntry(entry);
//		
//		
//		form = "{\n" + 
//				" \"symbolicName\": \"gattung_db\",\n" + 
//				" \"displayName\": \"Gattung (DB)\",\n" + 
//				" \"source\": \"PRODUCT\",\n" + 
//				" \"type\": \"_Text\"\n" + 
//				"}";
		
	}



	private void autoCompleteForJsonFieldValue(AcContext context, JsonMapper jsonMapper, JsonField fieldNode,
			JsonToken tokenLocked, int columnu8, ModuleDto test) {
		String fieldName = fieldNode.name();
		
		if ("value".equals(fieldName)) {
			autoCompleteForPropertyValue(context, jsonMapper, fieldNode, tokenLocked, columnu8, test);
		} else if ("symbolicName".equals(fieldName)) {
			autoCompleteForSymbolicName(context, jsonMapper, fieldNode, tokenLocked, columnu8, test);
			
		} else {
			Object output = fieldNode.getParentNode().getOutput();
			log("output="+output);
			if (output!=null) {
				TypeMapper<?> mapper = jsonMapper.getMapper(output.getClass());
				log("mapper="+mapper);
				if (mapper instanceof ObjectTypeMapper) {
					TypeMapper fieldValueMapper = ((ObjectTypeMapper) mapper).getFieldValueMapper(fieldName);
					log("fieldValueMapper="+fieldValueMapper);
					if (fieldValueMapper instanceof EnumTypeMapper) {
						EnumTypeMapper emapper = (EnumTypeMapper) fieldValueMapper;
						Enum[] enumConstants = emapper.getEnumConstants();
						List<String> enumNames = Arrays.stream(enumConstants).map(Enum::name).collect(Collectors.toList());
						Collections.sort(enumNames);
						for(String enumName : enumNames) {
							context.addEntry(new AcEntryReplaceText(enumName, enumName));
						}
					}
				}
			}
//			String indent="|| ";
//			JsonNode node = tokenLocked;
//			while(node!=null) {
//				log(indent+node+" :: "+node.getOutput());
//				node = node.getParentNode();
//				indent = indent+"  ";
//			}
		}
	}

	private JsonField detectParentArrayFieldNode(JsonField fieldNode) {
		JsonNode expectObjectNode = fieldNode.getParentNode();
		if (expectObjectNode instanceof JsonObject) {
			JsonNode expectArrayNode = expectObjectNode.getParentNode();
			if (expectArrayNode instanceof JsonArray) {
				JsonNode expectFieldNode = expectArrayNode.getParentNode();
				if (expectFieldNode instanceof JsonField) {
					return (JsonField) expectFieldNode;
				}
			}
		}
		return null;
	}
	

	private void autoCompleteForSymbolicName(AcContext context, JsonMapper jsonMapper, JsonField fieldNode,
			JsonToken tokenLocked, int columnu8, ModuleDto test) {

		JsonField parentArrayFieldNode = detectParentArrayFieldNode(fieldNode);
		if (parentArrayFieldNode != null) {
			String ownerName = parentArrayFieldNode.name();
			if ("propertyValues".equals(ownerName)) {

				
				AutoCompleteModel model = null;
				Object output = parentArrayFieldNode.getParentNode().getOutput();
				
				boolean forArticle = false;
				
				if (output instanceof TransportProductDto) {
					TransportProductDto product = (TransportProductDto) output;
					model = new AutoCompleteModel(jsonMapper, test);
					model.buildForTransportProduct(product);
				} 
				else if (output instanceof ArticleDto) {
					ArticleDto aritcle = (ArticleDto) output;
					model = new AutoCompleteModel(jsonMapper, test);
					model.buildForArticle(aritcle);
					forArticle = true;
				}
				
				if (model != null) {
					Set<String> definedNames = model.getDefinedPropertyValueNames();
					List<PropertyNameAndSource> namesAndSourceList = model.getDefinedPropertyNamesAndSourceList(forArticle);

					for(PropertyNameAndSource nameAndSource : namesAndSourceList) {
						context.addEntry(new AcEntryReplaceText(nameAndSource.name, nameAndSource.source.name(), !definedNames.contains(nameAndSource.name), nameAndSource.name));
					}
				}
			}
		}
		
	}



	private void autoCompleteForPropertyValue(AcContext context, JsonMapper jsonMapper, JsonField fieldNode, JsonToken tokenLocked,
			int columnu8, ModuleDto test) {
		JsonField parentArrayFieldNode = detectParentArrayFieldNode(fieldNode);
		if (parentArrayFieldNode!=null && "propertyValues".equals(parentArrayFieldNode.name())) {
			JsonObject objectNodePropValue = (JsonObject) fieldNode.getParentNode();
			
			Fpel fpel = new Fpel();
			String expression = tokenLocked.getValue().toString();
			List<FpelNode> path = null;
			if (expression.isEmpty()) {
				path = Collections.emptyList();
			} else {
				FpelExpression fpelExpression = fpel.compile(expression);
//					fpelExpression.dump();

				int columnInText = columnu8 - 1 - tokenLocked.getLeft().getUtf8offset();
				
				int actualColumn = Utf8.utf8offset(expression, columnInText);
				log("colInText="+columnInText+", column="+columnu8+", actualColumn="+actualColumn);
				log(expression.substring(0, columnInText) + "*"+ expression.substring(columnInText));
				
				path = fpelExpression.pathToLocation(actualColumn);
				
				log("path="+path);
			}
				
			
			Object output = parentArrayFieldNode.getParentNode().getOutput();
			log("output="+output);
			
			AutoCompleteModel model = null;
			
			if (output instanceof TransportProductDto) {
				TransportProductDto product = (TransportProductDto) output;
				model = new AutoCompleteModel(jsonMapper, test);
				model.buildForTransportProduct(product);
			} 
			else if (output instanceof ArticleDto) {
				ArticleDto aritcle = (ArticleDto) output;
				model = new AutoCompleteModel(jsonMapper, test);
				model.buildForArticle(aritcle);
			}
			
			if (model != null) {

				JsonField fieldSymbolicName = objectNodePropValue.getField("symbolicName");
				if (fieldSymbolicName!=null) {
					JsonValue value = fieldSymbolicName.getValue();
					if (value instanceof JsonSimpleValue) {
						Object evalue = ((JsonSimpleValue) value).getValue();
						if (evalue instanceof String) {
							String propertyName = (String) evalue;
							PropertyTypeDto propertyType = model.findPropertyType(propertyName);
							log("propertyType="+propertyType+", propertyName="+propertyName);
							if (propertyType instanceof EnumerationTypeDto) {
								EnumerationTypeDto ed = (EnumerationTypeDto) propertyType;
								List<String> enumNames = ed.getValues().stream().map(s -> s.getSymbolicName()).collect(Collectors.toList());
								Collections.sort(enumNames);
								for(String enumName : enumNames) {
									context.addEntry(new AcEntryReplaceText(enumName, enumName));
								}
								return;
							}
						}
					}
				}
				
				
				Route route = model;

				if (!path.isEmpty()) {
					FpelNode topNode = path.get(path.size()-1);
					
					FpelNode node = topNode;
					while(node != null) {
						if (node instanceof FpelGetProperty) {
							break;
						}
						node = node.getParentNode();
					}
					
					log("topNode="+topNode+" :: node="+node);
					
					if (node==null && topNode!=null && topNode.getParentNode() instanceof FpelArrayAccess) {
						route = routeForArrayOrMap(model, (FpelArrayAccess) topNode.getParentNode());
					} else if (node==topNode && topNode.getParentNode().getParentNode() instanceof FpelArrayAccess) {
						FpelArrayAccess arrayAcces = (FpelArrayAccess) topNode.getParentNode();
						
						log("should list offset-posibilities for arrayAcces="+arrayAcces);
						FpelEvaluable fieldArrayOffset = arrayAcces.getFieldArrayOffset();
						route = resolveRoute(model, fieldArrayOffset);
						if (route == model) {
							FpelEvaluable field = arrayAcces.getField();
							if (field instanceof FpelGetProperty) {
								FpelGetProperty fp = (FpelGetProperty) field;
								log("main accesor="+fp);
							}
						}
						
					} else if (node instanceof FpelGetProperty) {
						route = resolveRoute(model, node);
					}
				}

				Map<String, Route> children = route.children();
				ArrayList<String> names = new ArrayList<>(children.keySet());
				Collections.sort(names);
				
				boolean completingNonELValue = path.isEmpty();
				if (!completingNonELValue && path.size()==1) {
					FpelNode fpelNode = path.get(0);
					if (fpelNode instanceof FpelRawOuput) {
						
					} else {
						completingNonELValue = false;
					}
					
				}
				
				for(String routeName : names) {
					Route childRoute = children.get(routeName);
					AcEntry acEntry = null;
					if (completingNonELValue && (childRoute instanceof TypeField)) {
						
						String typename = ((TypeField) childRoute).type().typename();
						String acEntryName = childRoute.name() + " : "+typename;
						
						UowCompletePropertyName uow = new UowCompletePropertyName(childRoute.name());
						acEntry = new AcEntryRunUow(acEntryName, uow);
						
					} else {
						acEntry = createAcEntry(childRoute);
					}
					context.addEntry(acEntry);
				}
			}
		}
	}

	
	private Route routeForArrayOrMap(AutoCompleteModel model, FpelArrayAccess topNode) {
		FpelEvaluable leftRoute = topNode.getField();
//		FpelEvaluable left = topNode.getField();
//		Route leftRoute = resolveRoute(model, left);
//		log("leftRoute="+leftRoute);
//		if (leftRoute==null) {
//			return model;
//		}
		if ("propertyValues".equals(leftRoute.name())) {
			List<PropertyValueDto> listPropertyValues = model.listPropertyValues();
			log("listPropertyValues="+listPropertyValues);
			
			TypeDescription typeString = model.getTypeRegistry().getType("String");
			LiteralListRoute literalListRoute = new LiteralListRoute(typeString);
			listPropertyValues.stream().map(s -> new TypeField(typeString, "'"+s.getSymbolicName()+"'")).forEach(literalListRoute::addRoute);
			return literalListRoute;
		}
		return null;
	}

	public static class LiteralListRoute implements Route {

		public TypeDescription type;
		
		private Map<String, Route> map = new HashMap<String, Route>();
		
		public LiteralListRoute(TypeDescription type) {
			this.type = type;
		}
		
		public void addRoute(TypeField field) {
			map.put(field.name(), field);
		}
		
		@Override
		public TypeDescription type() {
			return type;
		}
		
		@Override
		public String name() {
			return null;
		}
		
		@Override
		public Map<String, Route> children() {
			return map;
		}
	}


	public Route resolveRoute(AutoCompleteModel model, FpelNode node) {
		Route route = model;
		FpelEvaluable evaluable = (FpelEvaluable) node;
		
		List<FpelEvaluable> evalPath = new ArrayList<FpelEvaluable>();
		while(evaluable != null) {
			evalPath.add(evaluable);
			FpelNode parent = evaluable.getLHS();
			evaluable = null;
			if (parent instanceof FpelGetProperty) {
				evaluable = (FpelGetProperty) parent;
			}
		}
		
		Collections.reverse(evalPath);
		log("evalPath = "+evalPath);

		evalPath.remove(evalPath.size()-1);
		
		for(FpelEvaluable eval : evalPath) {
			FpelGetProperty current = (FpelGetProperty) eval; 
			String name = current.getName();
			Map<String, Route> routes = route.children();
			Route routeExit = routes.get(name);
			if (routeExit != null) {
				route = routeExit;
			} else {
				// TODO not found
				return null;
			}
		}		
		return route;
	}

	private AcEntry createAcEntry(Route route) {
		TypeDescription type = route.type();
		if (route instanceof TypeField) {
			return new AcEntryReplaceText(route.name(), type.typename(), true, route.name());
//			return new AcEntryReplaceText(route.name() + " : "+type.typename(), route.name());
		} else if (route instanceof TypeMethod) {
			TypeMethod m = (TypeMethod) route;
			StringBuilder buf = new StringBuilder();
			StringBuilder buf2 = new StringBuilder();
			for(TypeField arg : m.getArguments()) {
				if (buf.length()>0) {
					buf.append(", ");
					buf2.append(", ");
				}
				buf.append("[%").append(arg.name()).append("%]");
				buf2.append(arg.type().typename()).append(" ").append(arg.name());
			}
			String form = route.name()+"("+buf+")";
			String acListName = route.name()+"("+buf2+") : ";
			if (m.type() == null) {
				acListName += "void"; 
			} else {
				acListName += m.type().typename();
			}
			
			
			return new AcEntryInsertRawTemplate(acListName, form);
		}
		return null;
	}


	private void autoCompletePropertyGroupsEntry(AcContext context, JsonMapper jsonMapper, JsonToken tokenLocked, ModuleDto module) {
		List<String> groupNames = module.getDynamicProductPropertyGroups().stream().map(s -> s.getSymbolicName()).collect(Collectors.toList());
		for(String propName : groupNames) {
			context.addEntry(new AcEntryReplaceText(propName));
		}
	}

	private void autoCompleteProducts(AcContext context, JsonMapper jsonMapper, JsonToken tokenLocked, ModuleDto module) {
		List<String> productNames = module.getTransportProducts().stream().map(p -> p.getSymbolicName()).collect(Collectors.toList());
		for(String propName : productNames) {
			context.addEntry(new AcEntryReplaceText(propName));
		}
	}


	private void autoCompleteForJsonField(AcContext context, JsonMapper jsonMapper, JsonField parentNode, JsonToken tokenLocked) {
		List<JsonNode> parentChildren = parentNode.getChildren();
		if (parentChildren.get(0) == tokenLocked) {
			// this is the name of the field
			JsonNode destNode = parentNode.getParentNode();
			if (destNode!=null) {
				Object output = destNode.getOutput();
				if (output!=null) {
					TypeMapper<?> typeMapper = jsonMapper.getMapper(output.getClass());
					log("got type mapper:"+typeMapper);
					ObjectTypeMapper otmapper = (ObjectTypeMapper) typeMapper;
					List<String> propertyNames = otmapper.getPropertyNames();
					for(String propName : propertyNames) {
						TypeMapper<?> fieldValueMapper = otmapper.getFieldValueMapper(propName);
						UowCompletePropertyName uowCompleteFieldKey = new UowCompletePropertyName(propName);
						AcEntryRunUow entry = new AcEntryRunUow(propName, uowCompleteFieldKey);
						context.addEntry(entry);
					}
				}
			}
		}
	}

	
}
