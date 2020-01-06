package net.natpad.brilliance.eos.autocomplete;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.CopyOnWriteArrayList;

public class TypeDescriptionStopPoint implements TypeDescription {

	private final List<TypeField> fields;
	
	public TypeDescriptionStopPoint(TypeRegistry typeRegistry) {
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
		return "StopPoint";
	}

	@Override
	public List<TypeField> fields() {
		return new CopyOnWriteArrayList<TypeField>(fields);
	}
	
}
