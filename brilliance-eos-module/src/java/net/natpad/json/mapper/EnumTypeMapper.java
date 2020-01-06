package net.natpad.json.mapper;

import net.natpad.json.JsonError;
import net.natpad.json.JsonSimpleValue;
import net.natpad.json.JsonValue;
import net.natpad.json.JsonError.Level;
import net.natpad.json.JsonError.Type;

public class EnumTypeMapper<T extends Enum> implements TypeMapper<T>{

	private final TypeMapperRegistry mapperRegistry;
	private final Class<T> enumType;
	
	public EnumTypeMapper(TypeMapperRegistry mapperRegistry, Class<T> enumType) {
		this.mapperRegistry = mapperRegistry;
		this.enumType = enumType;
	}
	
	public T[] getEnumConstants() {
		return enumType.getEnumConstants();
	}
	
	@Override
	public T map(JsonValue value) {
		if (value instanceof JsonSimpleValue) {
			JsonSimpleValue simpleValue = (JsonSimpleValue) value;
			switch (simpleValue.getType()) {
			case QUOTED_STRING: {
				String valueString = (String) simpleValue.getValue();
				T result = null;
				try {
					result = (T) Enum.valueOf(enumType, valueString);
				} catch(Exception e) {
					
				}
				if (result == null) {
					T[] enumConstants = enumType.getEnumConstants();
					if (enumConstants.length<8) {
						StringBuilder buf = new StringBuilder();
						for(T t : enumConstants) {
							if (buf.length()>0) {
								buf.append(',');
							}
							buf.append(t.name());
						}
						JsonError error = new JsonError(Type.MODEL, Level.ERROR, "Could not map the string '" + valueString + "' to the enum of type "+enumType.getSimpleName()+". Value should be one of:"+buf);
						simpleValue.getToken().addError(error);
					} else {
						JsonError error = new JsonError(Type.MODEL, Level.ERROR, "Could not map the string '" + valueString + "' to the enum of type "+enumType.getSimpleName());
						simpleValue.getToken().addError(error);
					}					
				}
				return result;
			}
			default:
				JsonError error = new JsonError(Type.MODEL, Level.ERROR, "Expected a String here got an " + simpleValue.getType() + " instead");
				simpleValue.getToken().addError(error);
			}
		}
		return null;
	}
	
}

