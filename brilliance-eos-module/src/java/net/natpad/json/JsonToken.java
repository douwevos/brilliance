package net.natpad.json;

import net.natpad.json.JsonScanner.Location;

public class JsonToken extends JsonNode {

	private JsonTokenType type;
	private final Location left;
	private final Location right;
	private final Object value;
	
	private JsonError errors[];
	
	public JsonToken(JsonTokenType type) {
		this.type = type;
		this.left = null;
		this.right = null;
		this.value = null;
	}

	public JsonToken(JsonTokenType type, Location left, Location right) {
		this.type = type;
		this.left = left;
		this.right = right;
		this.value = null;
	}
	
	
	public JsonToken(JsonTokenType type, Location left, Location right, Object value) {
		this.type = type;
		this.left = left;
		this.right = right;
		this.value = value;
	}

	public JsonTokenType getType() {
		return type;
	}
	
	public void setType(JsonTokenType type) {
		this.type = type;
	}
	
	public Location getLeft() {
		return left;
	}
	
	public Location getRight() {
		return right;
	}
	
	public Object getValue() {
		return value;
	}
	
	public void addError(JsonError error) {
		if (errors == null) {
			errors = new JsonError[1];
			errors[0] = error;
		} else {
			JsonError ers[] = new JsonError[errors.length+1];
			System.arraycopy(errors, 0, ers, 0, errors.length);
			ers[errors.length] = error;
			errors = ers;
		}
	}
	
	public JsonError[] getErrors() {
		return errors;
	}

	@Override
	public String toString() {
		if (value!=null) {
			return "JsonToken [type=" + type + ", left=" + left + ", right=" + right + ", value="+value+"]";
		}
		return "JsonToken [type=" + type + ", left=" + left + ", right=" + right + "]";
	}
}
