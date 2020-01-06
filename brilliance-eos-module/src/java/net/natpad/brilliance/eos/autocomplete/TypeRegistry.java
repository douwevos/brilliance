package net.natpad.brilliance.eos.autocomplete;

import java.time.LocalDateTime;
import java.util.HashMap;
import java.util.Map;

public class TypeRegistry {

	private final Map<String, TypeDescription> map = new HashMap<>();
	
	public TypeRegistry() {
		register(new TypeDescriptionString());
		register(new TypeDescriptionInteger());
//		register(new TypeDescriptionLocalDateTime(this));
		register(new TypeDescriptionStopPoint(this));
		register(new TypeDescriptionPropertyInteger(this));
		register(new TypeDescriptionPropertyText(this));
		register(new TypeDescriptionPropertyTextMultiline(this));
		
		register(new TypeDescriptionJavaType(LocalDateTime.class, this));
		
		
	}
	
	public void register(TypeDescription description) {
		map.put(description.typename(), description);
	}

	public TypeDescription getType(String typeName) {
		return map.get(typeName);
	}
	
}
