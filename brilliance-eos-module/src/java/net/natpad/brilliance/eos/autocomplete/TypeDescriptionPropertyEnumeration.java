package net.natpad.brilliance.eos.autocomplete;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.CopyOnWriteArrayList;

import com.tickeos.core.service.impl.module.dto.EnumerationTypeDto;
import com.tickeos.core.service.impl.module.dto.EnumerationValueDto;

public class TypeDescriptionPropertyEnumeration implements TypeDescription {

	private final List<TypeField> fields;
	
	private EnumerationTypeDto dto;
	
	private String typeName;

	public TypeDescriptionPropertyEnumeration(TypeRegistry typeRegistry, EnumerationTypeDto dto) {
		typeName = dto.getSymbolicName();
		List<EnumerationValueDto> values = dto.getValues();
		TypeDescription typeString = typeRegistry.getType("String");
		List<TypeField> lFields = new ArrayList<TypeField>();
		values.stream().map(v -> new TypeField(typeString, v.getSymbolicName()));
		fields = new CopyOnWriteArrayList<TypeField>(lFields);
	}

	@Override
	public String typename() {
		return typeName;
	}

	@Override
	public List<TypeField> fields() {
		return new CopyOnWriteArrayList<TypeField>(fields);
	}


}
