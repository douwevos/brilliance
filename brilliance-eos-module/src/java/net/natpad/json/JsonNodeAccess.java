package net.natpad.json;

public class JsonNodeAccess {

	private final JsonNode node;
	
	public JsonNodeAccess(JsonNode node) {
		this.node = node;
	}
	
	public JsonNodeAccess parent() {
		if (node == null) {
			return this;
		}
		return new JsonNodeAccess(node.getParentNode());
	}
	
	public JsonNode get() {
		return node;
	}
	
	public Object output() {
		return node==null ? null : node.getOutput(); 
	}
	
}
