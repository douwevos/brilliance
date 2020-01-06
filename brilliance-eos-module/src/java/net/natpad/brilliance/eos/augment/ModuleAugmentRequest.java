package net.natpad.brilliance.eos.augment;

import java.io.IOException;
import java.io.StringReader;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Objects;
import java.util.Set;
import java.util.stream.Collectors;

import com.tickeos.core.service.impl.module.dto.ArticleDto;
import com.tickeos.core.service.impl.module.dto.DynamicProductPropertyDto;
import com.tickeos.core.service.impl.module.dto.DynamicProductPropertyGroupDto;
import com.tickeos.core.service.impl.module.dto.ModuleDto;
import com.tickeos.core.service.impl.module.dto.ProductDto;
import com.tickeos.core.service.impl.module.dto.ProductPropertyTemplateDto;
import com.tickeos.core.service.impl.module.dto.PropertyValueDto;
import com.tickeos.core.service.impl.module.dto.SpatialValidityDescriptorDto;
import com.tickeos.core.service.impl.module.dto.TransportProductDto;
import com.tickeos.core.shared.StaticProperty;

import net.natpad.brilliance.caterpillar.StringWo;
import net.natpad.brilliance.chameleon.Document;
import net.natpad.brilliance.chameleon.Revision;
import net.natpad.brilliance.chameleon.RevisionReader;
import net.natpad.brilliance.dragonfly.AugmentRequest;
import net.natpad.brilliance.dragonfly.KeywordPrinter;
import net.natpad.brilliance.dragonfly.LineTag;
import net.natpad.brilliance.dragonfly.LineTagLevel;
import net.natpad.brilliance.dragonfly.LineTagType;
import net.natpad.json.JsonArray;
import net.natpad.json.JsonError;
import net.natpad.json.JsonError.Level;
import net.natpad.json.JsonError.Type;
import net.natpad.json.JsonField;
import net.natpad.json.JsonNode;
import net.natpad.json.JsonObject;
import net.natpad.json.JsonScanner.Location;
import net.natpad.json.JsonSimpleValue;
import net.natpad.json.JsonToken;
import net.natpad.json.JsonTokenType;
import net.natpad.json.JsonValue;
import net.natpad.json.mapper.JsonMapper;

public class ModuleAugmentRequest extends AugmentRequest {

	private Map<Integer, List<JsonError>> errorsPerLine = new HashMap<>();
	
	public ModuleAugmentRequest(Document document, Revision revision, StringWo markupSlotKey) {
		super(document, revision, markupSlotKey);
	}
	
	@Override
	public boolean runAugment(Revision revision, KeywordPrinter keywordPrinter, KeywordPrinter line_tag_printer) {
		

//		log("buf="+buf);

		try {
			RevisionReader revisionReader = new RevisionReader(revision);
			String buf = readText(revisionReader);
			JsonMapper jsonMapper = new JsonMapper(new StringReader(buf));
			ModuleDto test = jsonMapper.map(ModuleDto.class);
			
			
			List<JsonToken> tokenList = jsonMapper.getTokenList();
			validateModel(test, tokenList, jsonMapper.getParsedValue());
			
			for(JsonToken token : tokenList) {
				Location left = token.getLeft();
				Location right = token.getRight();
				keywordPrinter.printFgColor(left.getRow(), left.getUtf8offset(), right.getRow(), right.getUtf8offset(), mapTokenToColor(token.getType()));

				JsonError[] errors = token.getErrors();
				if (errors==null) {
					continue;
				}
				
				for(JsonError error : errors) {
					LineTagType tagType = LineTagType.SCANNER;
					switch(error.getType()) {
						case MODEL : 
							tagType = LineTagType.MODEL;
							break;
						case PARSER : 
							tagType = LineTagType.PARSER;
							break;
						case SCANNER : 
							tagType = LineTagType.SCANNER;
							break;
					}
					LineTagLevel tagLevel = LineTagLevel.ERROR;
					switch(error.getLevel()) {
						case ERROR : tagLevel = LineTagLevel.ERROR; break;
						case INFO : tagLevel = LineTagLevel.INFO; break;
						case UNSAFE : tagLevel = LineTagLevel.UNSAFE; break;
						case WARN : tagLevel = LineTagLevel.WARN; break;
					}
					LineTag tag = new LineTag(left.getRow(), tagType, tagLevel);
					tag.setText(error.getMessage());
					
					tag.setStartAndEndIndex(left.getUtf8offset(), right.getUtf8offset());
					line_tag_printer.printLineTag(tag);
				}
			}
			
			
		} catch (Throwable e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
		return true;
	}


	private void validateModel(ModuleDto module, List<JsonToken> tokenList, JsonValue jsonRootValue) {

		validateDynamicProductPropertyGroups(module, jsonRootValue);
		
		validateTransportProducts(module, jsonRootValue);
	}

	private void validateTransportProducts(ModuleDto module, JsonValue jsonRootValue) {

		List<TransportProductDto> transportProducts = module.getTransportProducts();
		for(TransportProductDto prodDto : transportProducts) {
			
			// TODO: dangerous cast !!
			JsonObject prodJson = (JsonObject) findNodeWithValue(jsonRootValue, prodDto);
			JsonField field = prodJson.getField("propertyGroups");
			JsonArray groupArrayJson = null;
			if (field!=null) {
				// TODO: dangerous cast !!
				groupArrayJson = (JsonArray) field.getValue();
			}
			
			Set<String> referredProperties = new HashSet<String>(); 
			
			List<String> propertyGroups = prodDto.getPropertyGroups();
			List<DynamicProductPropertyGroupDto> groups = new ArrayList<DynamicProductPropertyGroupDto>();
			for(String groupName : propertyGroups) {
				DynamicProductPropertyGroupDto groupDto = findDynamicProductPropertyGroup(module, groupName);
				if (groupDto == null) {
					JsonSimpleValue v = (JsonSimpleValue) groupArrayJson.getValues().stream().filter(s -> s instanceof JsonSimpleValue).filter(s -> {
						return Objects.equals(((JsonSimpleValue) s).getValue(), groupName);
					}).findAny().orElse(null);
					
					if (v != null) {
						JsonToken token = (JsonToken) v.getToken();
						JsonError jsonError = new JsonError(Type.MODEL,  Level.ERROR, "Unknown group");
						token.addError(jsonError);
					}
				} else {
					groups.add(groupDto);
				}
			}
			
			List<String> propertyTemplates = new ArrayList<>();
			
			String spatialValidityDescriptor = prodDto.getSpatialValidityDescriptor();
			if (spatialValidityDescriptor != null) {

				SpatialValidityDescriptorDto descriptor = module.getSpatialValidityDescriptors().stream().filter(s -> Objects.equals(spatialValidityDescriptor, s.getSymbolicName())).findAny().orElse(null);
				if (descriptor == null) {
					JsonField fieldSvd = prodJson.getField("spatialValidityDescriptor");
					// TODO: dangerous cast !!
					JsonToken svdName = (JsonToken) fieldSvd.getValue().getChildren().get(0);
					JsonError jsonError = new JsonError(Type.MODEL, Level.ERROR, "Spatial-Validity descriptor not defined");
					svdName.addError(jsonError);
				} else {
					
					if (descriptor.getStart()!=null) {
						propertyTemplates.add(StaticProperty.REQUESTED_START.getSymbolicName());
					}
					if (descriptor.getDestination()!=null) {
						propertyTemplates.add(StaticProperty.REQUESTED_DESTINATION.getSymbolicName());
					}
					if (descriptor.getArea()!=null) {
						propertyTemplates.add(StaticProperty.AREA.getSymbolicName());
					}
				}
			}
			
			
			List<PropertyValueDto> values = prodDto.getPropertyValues();
			for(PropertyValueDto value : values) {
				String valueSymbolicName = value.getSymbolicName();
				List<DynamicProductPropertyDto> properties = findProductProperties(groups, valueSymbolicName);
				List<String> propertyTemplatesM = findPropertyTemplates(propertyTemplates, valueSymbolicName);
				
				if (properties.isEmpty() && propertyTemplatesM.isEmpty()) {
					// TODO: dangerous cast !!
					JsonNode propValueJson = findNodeWithValue(jsonRootValue, value);
					markErrorAtSymbolicName(propValueJson, Level.ERROR, "Property name not defined");
				} else {
					referredProperties.add(valueSymbolicName);
				}
			}
			
			List<ArticleDto> articles = prodDto.getArticles();
			if (articles != null) {
				for(ArticleDto articleDto : articles) {
					validateArticle(articleDto, groups, propertyTemplates, referredProperties, jsonRootValue);
				}
			}
			
		}
	}


	private List<String> findPropertyTemplates(List<String> propertyTemplates,
			String valueSymbolicName) {
		return propertyTemplates.stream().filter(s -> Objects.equals(s, valueSymbolicName)).collect(Collectors.toList());
	}

	private void validateArticle(ArticleDto articleDto, List<DynamicProductPropertyGroupDto> groups,
			List<String> propertyTemplates, Set<String> referredProperties, JsonValue jsonRootValue) {
	
		Set<String> localReferredProperties = new HashSet<String>(referredProperties);
		
		List<PropertyValueDto> values = articleDto.getPropertyValues();
		for(PropertyValueDto value : values) {
			String valueSymbolicName = value.getSymbolicName();
			List<DynamicProductPropertyDto> properties = findProductProperties(groups, valueSymbolicName);
			List<String> propertyTemplatesM = findPropertyTemplates(propertyTemplates, valueSymbolicName);
			
			if (properties.isEmpty() && propertyTemplatesM.isEmpty()) {
				// TODO: dangerous cast !!
				JsonNode propValueJson = findNodeWithValue(jsonRootValue, value);
				markErrorAtSymbolicName(propValueJson, Level.ERROR, "Property name not defined");
			} else {
				localReferredProperties.add(valueSymbolicName);
			}
		}
		
		
		JsonObject articleNode = (JsonObject) findNodeWithValue(jsonRootValue, articleDto);
		JsonField field = articleNode.getField("propertyValues");
		if (field!=null) {
			JsonToken propertValuesKeyToken = field.getKey();
			
			for(DynamicProductPropertyGroupDto group : groups) {
				List<DynamicProductPropertyDto> missingProperties = group.getProperties().stream().filter(s -> !localReferredProperties.contains(s.getSymbolicName())).collect(Collectors.toList());
				if (!missingProperties.isEmpty()) {
					StringBuilder buf = new StringBuilder();
					for(DynamicProductPropertyDto pdto : missingProperties) {
						if (buf.length()>0) {
							buf.append(",");
						}
						buf.append(pdto.getSymbolicName());
					}
	
					String message = "The properties "+buf+" are defined by the group "+group.getSymbolicName()+" are not defined for the Article";
					JsonError jsonError = new JsonError(Type.MODEL, Level.ERROR, message);
					propertValuesKeyToken.addError(jsonError);
				}
			}
		}
		

	}

	private List<DynamicProductPropertyDto> findProductProperties(List<DynamicProductPropertyGroupDto> groups,
			String valueSymbolicName) {
		List<DynamicProductPropertyDto> result = new ArrayList<DynamicProductPropertyDto>();
		for(DynamicProductPropertyGroupDto groupDto : groups) {
			DynamicProductPropertyDto property = groupDto.getProperties().stream().filter(s -> Objects.equals(s.getSymbolicName(), valueSymbolicName)).findAny().orElse(null);
			if (property!=null) {
				result.add(property);
			}
		}
		return result;
	}

	private DynamicProductPropertyGroupDto findDynamicProductPropertyGroup(ModuleDto module, String groupName) {
		return module.getDynamicProductPropertyGroups().stream()
				.filter(s -> Objects.equals(s.getSymbolicName(), groupName)).findAny().orElse(null);
	}

	private void validateDynamicProductPropertyGroups(ModuleDto module, JsonValue jsonRootValue) {
		HashSet<String> propertyGroupsUsed = new HashSet<>();
		List<TransportProductDto> transportProducts = module.getTransportProducts();
		for(TransportProductDto prodDto : transportProducts) {
			List<String> propertyGroups = prodDto.getPropertyGroups();
			propertyGroupsUsed.addAll(propertyGroups);
		}
		
		List<ProductDto> products = module.getProducts();
		for(ProductDto prodDto : products) {
			List<String> propertyGroups = prodDto.getPropertyGroups();
			propertyGroupsUsed.addAll(propertyGroups);
		}
		

		List<DynamicProductPropertyGroupDto> dynamicProductPropertyGroups = module.getDynamicProductPropertyGroups();
		for(DynamicProductPropertyGroupDto dto : dynamicProductPropertyGroups) {
			String groupName = dto.getSymbolicName();
			if (propertyGroupsUsed.contains(groupName)) {
				continue;
			}
			
			
			JsonNode node = findNodeWithValue(jsonRootValue, dto);
			markErrorAtSymbolicName(node, Level.WARN, "Unused group");
			
		}
	}

	private void markErrorAtSymbolicName(JsonNode node, Level level, String message) {
		if (node!=null) {
			if (node instanceof JsonObject) {
				JsonObject jo = (JsonObject) node;
				JsonField field = jo.getField("symbolicName");
				if (field!=null) {
					JsonValue value = field.getValue();
					if (value instanceof JsonSimpleValue) {
						JsonSimpleValue simpleValue = (JsonSimpleValue) value;
						JsonToken token = (JsonToken) simpleValue.getChildren().get(0);
						JsonError jsonError = new JsonError(Type.MODEL, level, message);
						token.addError(jsonError);
					}
				}
			}
			
		}
	}


	private JsonNode findNodeWithValue(JsonNode jsonRootNode, Object object) {
		if (jsonRootNode==null) {
			return null;
		}
		if (jsonRootNode.getOutput() == object) {
			return jsonRootNode;
		}
		List<JsonNode> children = jsonRootNode.getChildren();
		if (children!=null) {
			for(JsonNode child : children) {
				JsonNode result = findNodeWithValue(child, object);
				if (result!=null) {
					return result;
				}
			}
		}
		return null;
	}


	public static String readText(RevisionReader revisionReader) {
		StringBuilder buf = new StringBuilder();
		while(true) {
			int utf8Char = revisionReader.scanUtf8Char();
			if (utf8Char==-1) {
				break;
			}
			buf.appendCodePoint(utf8Char);
		}
		return buf.toString();
	}


	private int mapTokenToColor(JsonTokenType type) {
		switch(type) {
			case COLON : return 4;
			case COMMA : return 5;
			case NUMBER : return 6;
			case TRUE : return 8;
			case FALSE : return 8;
			case NULL : return 8;
			case LEFT_BRACE : return 10;
			case RIGHT_BRACE : return 10;
			case LEFT_BRACKET : return 11;
			case RIGHT_BRACKET : return 11;
			
			case QUOTED_STRING : return 1;
			case ATTRIBUTE_NAME : return 9;
		}
		return 2;
	}
	
}
