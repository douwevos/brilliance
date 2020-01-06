package net.natpad.json;

import java.util.ArrayList;
import java.util.List;

public class JsonField extends JsonNode {

	private final JsonToken key;
	private final JsonValue value;
	
	private JsonField(List<JsonNode> children, JsonToken key, JsonValue value) {
		super(children);
		key.setType(JsonTokenType.ATTRIBUTE_NAME);
		this.key = key;
		this.value = value;
	}
	
	public JsonToken getKey() {
		return key;
	}
	
	public JsonValue getValue() {
		return value;
	}
	
	
	public String name() {
		return (String) key.getValue();
	}
	
	public static JsonFieldBuilder builder() {
		return new JsonFieldBuilder();
	}
	
	public static class JsonFieldBuilder implements JsonNodeCollector {
		
		private List<JsonNode> nodes = new ArrayList<>();

		private JsonToken key;
		private JsonValue value;
		
		@Override
		public void append(JsonNode node) {
			nodes.add(node);
		}
		
		public void setKey(JsonToken key) {
			this.key = key;
			nodes.add(key);
		}
		
		public void setValue(JsonValue value) {
			this.value = value;
			nodes.add(value);
		}
		
		public JsonField build() {
			return new JsonField(nodes, key, value);
		}
		
	}

	@Override
	public String toString() {
		return "JsonField["+name()+"]";
	}
	
}
