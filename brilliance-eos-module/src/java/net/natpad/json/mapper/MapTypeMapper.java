package net.natpad.json.mapper;

import java.lang.reflect.Type;
import java.util.HashMap;
import java.util.Map;

import net.natpad.json.JsonField;
import net.natpad.json.JsonObject;
import net.natpad.json.JsonValue;

public class MapTypeMapper<T extends Map<?, ?>> implements TypeMapper<T> {


	private final TypeMapperRegistry mapperRegistry;
	private final Type rawType;
	private final Type elementKeyType;
	private final Type elementValueType;
	
	private TypeMapper<?> elementKeyMapper; 
	private TypeMapper<?> elementValueMapper; 

	public MapTypeMapper(TypeMapperRegistry mapperRegistry, Type rawType, Type elementKeyType, Type elementValueType) {
		this.mapperRegistry = mapperRegistry;
		this.rawType = rawType;
		this.elementKeyType = elementKeyType;
		this.elementValueType = elementValueType;
	}

	@Override
	public void addNotify() {
		TypeMapper.super.addNotify();
		elementKeyMapper = mapperRegistry.getMapper(elementKeyType);
		elementValueMapper = mapperRegistry.getMapper(elementValueType);
	}

	@SuppressWarnings({ "rawtypes", "unchecked" })
	@Override
	public T map(JsonValue value) {
		HashMap result = null;
		if (value instanceof JsonObject) {
			try {
//				Constructor constructor = ((Class) rawType).getDeclaredConstructor();
				result = new HashMap();
				JsonObject jsonArray = (JsonObject) value;
				for(JsonField field : jsonArray) {
					String key = field.getKey().getValue().toString();
					
					Object valuem = elementValueMapper.map(field.getValue());
					result.put(key, valuem);
				}
			} catch (SecurityException | IllegalArgumentException e) {
				e.printStackTrace();
			}
		}
		return (T) result;
	}
	
}
