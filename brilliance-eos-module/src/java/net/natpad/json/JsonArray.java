package net.natpad.json;

import java.util.ArrayList;
import java.util.List;
import java.util.stream.Collectors;

public class JsonArray extends JsonValue {

	private final List<JsonValue> values;
	
	private JsonArray(List<JsonNode> values) {
		super(values);
		this.values = values.stream().filter(p -> p instanceof JsonValue).map(s -> (JsonValue) s).collect(Collectors.toList());
	}

	
	public List<JsonValue> getValues() {
		return values;
	}

	public static JsonArrayBuilder builder() {
		return new JsonArrayBuilder();
	}
	
	public static class JsonArrayBuilder implements JsonNodeCollector {
		private List<JsonNode> values = new ArrayList<JsonNode>();
		
		
		@Override
		public void append(JsonNode node) {
			values.add(node);
		}
		
		public JsonArray build() {
			return new JsonArray(values);
		}
		
	}
}
