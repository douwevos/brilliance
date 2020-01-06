package net.natpad.json.mapper;

import java.lang.reflect.Type;

public interface TypeMapperRegistry {

	TypeMapper<?> getMapper(Type type);
	
}
