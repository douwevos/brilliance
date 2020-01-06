package net.natpad.brilliance.eos.autocomplete;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.CopyOnWriteArrayList;

public class TypeDescriptionPropertyTextMultiline implements TypeDescription {

	private final List<TypeField> fields;

	public TypeDescriptionPropertyTextMultiline(TypeRegistry typeRegistry) {
		TypeDescription typeString = typeRegistry.getType("String");
		TypeDescription typeInteger = typeRegistry.getType("Integer");
		List<TypeField> lFields = new ArrayList<TypeField>();
		lFields.add(new TypeField(typeString, "displayName"));
		lFields.add(new TypeField(typeString, "symbolicName"));
		lFields.add(new TypeField(typeString, "pattern"));
		lFields.add(new TypeField(typeInteger, "minLength"));
		lFields.add(new TypeField(typeInteger, "maxLength"));
		fields = new CopyOnWriteArrayList<TypeField>(lFields);
	}

	@Override
	public String typename() {
		return "_TextMultiline";
	}

	@Override
	public List<TypeField> fields() {
		return new CopyOnWriteArrayList<TypeField>(fields);
	}


}
