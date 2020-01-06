package net.natpad.json.mapper;

import java.io.IOException;
import java.io.Reader;
import java.lang.reflect.ParameterizedType;
import java.lang.reflect.Type;
import java.math.BigDecimal;
import java.time.LocalDate;
import java.time.LocalDateTime;
import java.time.LocalTime;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import net.natpad.json.JsonParser;
import net.natpad.json.JsonScanner;
import net.natpad.json.JsonToken;
import net.natpad.json.JsonValue;

public class JsonMapper implements TypeMapperRegistry {

	private final JsonScanner scanner;
	private final JsonParser parser;
	
	private JsonValue parsedValue;
	
	private Map<Type, TypeMapper<?>> mappers = new HashMap<>();
	
	public JsonMapper(Reader reader) throws IOException {
		scanner = new JsonScanner(reader);
		parser = new JsonParser(scanner);
		registerMapper(String.class, new TypeMapperString());
		registerMapper(Boolean.class, new TypeMapperBoolean());
		registerMapper(boolean.class, new TypeMapperBoolean());
		registerMapper(Integer.class, new TypeMapperInteger());
		registerMapper(int.class, new TypeMapperInteger());
		registerMapper(Long.class, new TypeMapperLong());
		registerMapper(long.class, new TypeMapperLong());
		registerMapper(BigDecimal.class, new TypeMapperBigDecimal());
		registerMapper(LocalDateTime.class, new TypeMapperLocalDateTime());
		registerMapper(LocalDate.class, new TypeMapperLocalDate());
		registerMapper(LocalTime.class, new TypeMapperLocalTime());
		
	}
	
	private <T> void registerMapper(Type type, TypeMapper<T> typeMapper) {
		mappers.put(type, typeMapper);
		typeMapper.addNotify();
	}

	public <T> T map(Class<T> toType) throws IOException {
		parsedValue = parser.parse();
		return map(parsedValue, toType);
	}
	
	public JsonValue getParsedValue() {
		return parsedValue;
	}

	@SuppressWarnings("unchecked")
	private <T> T map(JsonValue value, Class<T> toType) {
		TypeMapper<?> typeMapper = getMapper(toType);
		if (typeMapper == null) {
			typeMapper = new ObjectTypeMapper(this, toType);
			registerMapper(toType, typeMapper);
		}
		return (T) typeMapper.map(value);
	}
	
	@Override
	public TypeMapper<?> getMapper(Type type) {
		TypeMapper<?> result = mappers.get(type);
		if (result == null) {
			result = createMapper(type);
			if (result !=null) {
				registerMapper(type, result);
			}
		}
		return result;
	}

	private TypeMapper<?> createMapper(Type type) {
		if (type instanceof ParameterizedType) {
			ParameterizedType ap = (ParameterizedType) type;
			if (ap.getRawType() instanceof Class) {
				if (List.class.isAssignableFrom((Class) ap.getRawType())) {
					return createListMapper(ap);
				} else if (Map.class.isAssignableFrom((Class) ap.getRawType())) {
					return createMapMapper(ap);
				}
			}
			
		} else if (type instanceof Class) {
			Class c = (Class) type;
			if (c.isEnum()) {
				return new EnumTypeMapper(this, c);
			}
			return new ObjectTypeMapper(this,  type);
		}
		return null;
	}

	private TypeMapper createListMapper(ParameterizedType ap) {
		Type rawType = ap.getRawType();
		Type elementType = ap.getActualTypeArguments()[0];
		return new ListTypeMapper(this, rawType, elementType);
		
	}

	private TypeMapper createMapMapper(ParameterizedType ap) {
		Type rawType = ap.getRawType();
		Type[] actualTypeArguments = ap.getActualTypeArguments();
		Type elementKeyType = actualTypeArguments[0];
		Type elementValueType = actualTypeArguments[1];
		return new MapTypeMapper(this, rawType, elementKeyType, elementValueType);
		
	}

	public List<JsonToken> getTokenList() {
		return parser.getTokenList();
	}

	
}
