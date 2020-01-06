package net.natpad.json;

import java.util.List;
import java.util.concurrent.CopyOnWriteArrayList;

public class JsonNode {
	
	private final List<JsonNode> children;
	private JsonNode parentNode;
	private Object output;
	
	public JsonNode() {
		children = null;
	}
	
	public JsonNode(List<JsonNode> children) {
		this.children = new CopyOnWriteArrayList<JsonNode>(children);
		if (children!=null) {
			children.stream().filter(s -> s!=null).forEach(s -> s.parentNode = this);
		}
	}
	
	public JsonNode getParentNode() {
		return parentNode;
	}
	
	public List<JsonNode> getChildren() {
		return children == null ? null : new CopyOnWriteArrayList<JsonNode>(children);
	}
	
	
	public Object getOutput() {
		return output;
	}
	
	public void setOutput(Object output) {
		this.output = output;
	}
}
