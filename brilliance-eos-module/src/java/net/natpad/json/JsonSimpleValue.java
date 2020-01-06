package net.natpad.json;

import java.util.Arrays;

public class JsonSimpleValue extends JsonValue {

	private final JsonToken token;
	
	public JsonSimpleValue(JsonToken token) {
		super(Arrays.asList(token));
		this.token = token;
	}
	
	
	public JsonToken getToken() {
		return token;
	}

	public Object getValue() {
		return token.getValue();
	}

	public JsonTokenType getType() {
		return token.getType();
	}

	
}
