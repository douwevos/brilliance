package net.natpad.json.mapper;

import net.natpad.json.JsonError;
import net.natpad.json.JsonSimpleValue;
import net.natpad.json.JsonValue;
import net.natpad.json.JsonError.Level;
import net.natpad.json.JsonError.Type;

public class TypeMapperString implements TypeMapper<String> {

	@Override
	public String map(JsonValue value) {
		if (value instanceof JsonSimpleValue) {
			JsonSimpleValue simpleValue = (JsonSimpleValue) value;
			switch (simpleValue.getType()) {
			case QUOTED_STRING: {
				return (String) simpleValue.getValue();
			}
			default:
				JsonError error = new JsonError(Type.MODEL, Level.ERROR, "Expected a String here got an " + simpleValue.getType() + " instead");
				simpleValue.getToken().addError(error);
			}
		}
		
		return null;
	}
}
