package net.natpad.json;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.Objects;
import java.util.stream.Collectors;

public class JsonObject extends JsonValue implements Iterable<JsonField> {

	private List<JsonField> values;

	private JsonObject(List<JsonNode> nodes) {
		super(nodes);
		this.values = (List) nodes.stream().filter(p -> p instanceof JsonField).collect(Collectors.toList());
	}

	public JsonField getField(String name) {
		return values.stream().filter(p -> Objects.equals(p.name(), name)).findAny().orElse(null);
	}
	
	@Override
	public Iterator<JsonField> iterator() {
		return values.iterator();
	}
	
	public static JsonObjectBuilder builder() {
		return new JsonObjectBuilder();
	}
	
	public static class JsonObjectBuilder implements JsonNodeCollector {
		private final List<JsonNode> nodes = new ArrayList<JsonNode>();
		
		@Override
		public void append(JsonNode node) {
			nodes.add(node);
		}
		
		public JsonObject build() {
			return new JsonObject(nodes);
		}
	}
	
	@Override
	public String toString() {
		List<String> names = values.stream().map(s -> s.name()).collect(Collectors.toList());
		return "JsonObject["+names+"]";
	}

}
