package net.natpad.json.mapper;

import java.time.LocalDateTime;

import net.natpad.json.JsonError;
import net.natpad.json.JsonSimpleValue;
import net.natpad.json.JsonValue;
import net.natpad.json.JsonError.Level;
import net.natpad.json.JsonError.Type;

public class TypeMapperLocalDateTime implements TypeMapper<LocalDateTime> {

	@Override
	public LocalDateTime map(JsonValue value) {
		if (value instanceof JsonSimpleValue) {
			JsonSimpleValue simpleValue = (JsonSimpleValue) value;
			switch (simpleValue.getType()) {
			case QUOTED_STRING: {
				return LocalDateTime.parse((String) simpleValue.getValue());
			}
			default:
				JsonError error = new JsonError(Type.MODEL, Level.ERROR, "Expected a LocalDateTime here got an " + simpleValue.getType() + " instead");
				simpleValue.getToken().addError(error);
			}
		}
		
		return null;
	}

}
