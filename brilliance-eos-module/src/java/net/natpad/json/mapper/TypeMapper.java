package net.natpad.json.mapper;

import net.natpad.json.JsonValue;

public interface TypeMapper<T> {

	public T map(JsonValue value);
	
	default void addNotify() {}
	
}
