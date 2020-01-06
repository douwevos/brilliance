package net.natpad.brilliance.eos.autocomplete;

import java.util.HashMap;
import java.util.Map;

public class TypeField implements Route {

	private final TypeDescription type;
	private final String fieldName;
	
	public TypeField(TypeDescription type, String fieldName) {
		this.type = type;
		this.fieldName = fieldName;
	}

	@Override
	public String name() {
		return fieldName;
	}

	@Override
	public TypeDescription type() {
		return type;
	}
	
	@Override
	public Map<String, Route> children() {
		Map<String, Route> result = new HashMap<String, Route>();
		type.fields().stream().forEach(p -> result.put(p.name(), p));
		return result;
	}
	
	
}
