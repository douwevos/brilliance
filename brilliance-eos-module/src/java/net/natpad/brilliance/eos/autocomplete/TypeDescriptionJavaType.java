package net.natpad.brilliance.eos.autocomplete;

import java.lang.reflect.Field;
import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.CopyOnWriteArrayList;

public class TypeDescriptionJavaType implements TypeDescription {

	private final String typename;
	private final List<TypeField> fields;
	
	public TypeDescriptionJavaType(Class<?> type, TypeRegistry registry) {
		typename = type.getSimpleName();
		Field[] declaredFields = type.getDeclaredFields();
		List<TypeField> fieldsList = new ArrayList<TypeField>();
		for(Field field : declaredFields) {
			if (field.isAccessible()) {
				String name = field.getName();
				
				TypeDescription fieldType = registry.getType("String");
				TypeField typeField = new TypeField(fieldType, name);
				fieldsList.add(typeField);
			}
		}
		fields = new CopyOnWriteArrayList<TypeField>(fieldsList);
	}
	
	@Override
	public String typename() {
		return typename;
	}
	
	@Override
	public List<TypeField> fields() {
		return new CopyOnWriteArrayList<TypeField>(fields);
	}
	
	
}
