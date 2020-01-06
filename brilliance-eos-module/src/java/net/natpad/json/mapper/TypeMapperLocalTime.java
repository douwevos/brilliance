package net.natpad.json.mapper;

import java.time.LocalTime;

import net.natpad.json.JsonError;
import net.natpad.json.JsonSimpleValue;
import net.natpad.json.JsonValue;
import net.natpad.json.JsonError.Level;
import net.natpad.json.JsonError.Type;

public class TypeMapperLocalTime implements TypeMapper<LocalTime> {

	@Override
	public LocalTime map(JsonValue value) {
		if (value instanceof JsonSimpleValue) {
			JsonSimpleValue simpleValue = (JsonSimpleValue) value;
			switch (simpleValue.getType()) {
			case QUOTED_STRING: {
				return LocalTime.parse((String) simpleValue.getValue());
			}
			default:
				JsonError error = new JsonError(Type.MODEL, Level.ERROR, "Expected a LocalTime here got an " + simpleValue.getType() + " instead");
				simpleValue.getToken().addError(error);
			}
		}
		
		return null;
	}

}
