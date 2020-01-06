package net.natpad.brilliance.eos.autocomplete;

import java.time.LocalDateTime;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.CopyOnWriteArrayList;

public class TypeDescriptionLocalDateTime implements TypeDescription {

	private final List<TypeField> fields;
	
	public TypeDescriptionLocalDateTime(TypeRegistry typeRegistry) {
		LocalDateTime s;
		
		TypeDescription typeString = typeRegistry.getType("String");
		List<TypeField> lFields = new ArrayList<TypeField>();
		lFields.add(new TypeField(typeString, "pointId"));
		lFields.add(new TypeField(typeString, "name"));
		lFields.add(new TypeField(typeString, "region"));
		lFields.add(new TypeField(typeString, "globalId"));
		fields = new CopyOnWriteArrayList<TypeField>(lFields);
	}

	@Override
	public String typename() {
		return "LocalDateTime";
	}

	@Override
	public List<TypeField> fields() {
		return new CopyOnWriteArrayList<TypeField>(fields);
	}
	
}
