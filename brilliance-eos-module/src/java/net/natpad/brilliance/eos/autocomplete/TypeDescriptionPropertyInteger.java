package net.natpad.brilliance.eos.autocomplete;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.CopyOnWriteArrayList;

public class TypeDescriptionPropertyInteger implements TypeDescription {

	private final List<TypeField> fields;

	public TypeDescriptionPropertyInteger(TypeRegistry typeRegistry) {
		TypeDescription typeInteger = typeRegistry.getType("Integer");
		List<TypeField> lFields = new ArrayList<TypeField>();
		lFields.add(new TypeField(typeInteger, "minValue"));
		lFields.add(new TypeField(typeInteger, "maxValue"));
		fields = new CopyOnWriteArrayList<TypeField>(lFields);
	}

	@Override
	public String typename() {
		return "_Integer";
	}

	@Override
	public List<TypeField> fields() {
		return new CopyOnWriteArrayList<TypeField>(fields);
	}


}
