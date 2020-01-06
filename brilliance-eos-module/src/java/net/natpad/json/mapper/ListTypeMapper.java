package net.natpad.json.mapper;

import java.lang.reflect.Constructor;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Type;
import java.util.ArrayList;
import java.util.List;

import net.natpad.json.JsonArray;
import net.natpad.json.JsonValue;

public class ListTypeMapper<T extends List<?>> implements TypeMapper<T> {

	private final TypeMapperRegistry mapperRegistry;
	private final Type rawType;
	private final Type elementType;
	
	private TypeMapper<?> elementMapper; 
	
	public ListTypeMapper(TypeMapperRegistry mapperRegistry, Type rawType, Type elementType) {
		this.mapperRegistry = mapperRegistry;
		this.rawType = rawType;
		this.elementType = elementType;
	}

	@Override
	public void addNotify() {
		TypeMapper.super.addNotify();
		elementMapper = mapperRegistry.getMapper(elementType);
	}
	
	@SuppressWarnings({ "rawtypes", "unchecked" })
	@Override
	public T map(JsonValue value) {
		List result = null;
		if (value instanceof JsonArray) {
			try {
//				Constructor constructor = ((Class) rawType).getDeclaredConstructor();
				result = new ArrayList();
				JsonArray jsonArray = (JsonArray) value;
				List<JsonValue> values = jsonArray.getValues();
				if (values != null) {
					for(JsonValue sub : values) {
						Object evalue = elementMapper.map(sub);
						sub.setOutput(evalue);
						result.add(evalue);
					}
				}
			} catch (SecurityException | IllegalArgumentException e) {
				e.printStackTrace();
			}
		}
		value.setOutput(result);
		return (T) result;
	}

}
