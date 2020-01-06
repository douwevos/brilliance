package net.natpad.json.mapper;

import java.lang.reflect.AnnotatedType;
import java.lang.reflect.Constructor;
import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.lang.reflect.Type;
import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import net.natpad.brilliance.Logger;
import net.natpad.json.JsonError;
import net.natpad.json.JsonField;
import net.natpad.json.JsonNode;
import net.natpad.json.JsonObject;
import net.natpad.json.JsonValue;
import net.natpad.json.JsonError.Level;

public class ObjectTypeMapper implements TypeMapper, Logger {

	private final TypeMapperRegistry mapperRegistry;
	private final Type type;
	
	private Constructor<?> constructor;
	
	private Map<String, PropertyMapper> fieldMappers = new HashMap<>();
	
	interface PropertyMapper {

		void map(Object instance, JsonValue propertyValue) throws ReflectiveOperationException;

		TypeMapper getValueTypeMapper();
	}
	
	static class FieldMapper implements PropertyMapper {
		
		private final TypeMapper<?> mapper;
		private final Field field;
		
		public FieldMapper(TypeMapper<?> mapper, Field field) {
			this.mapper = mapper;
			this.field = field;
			field.setAccessible(true);
		}
		
		@Override
		public void map(Object instance, JsonValue propertyValue) throws ReflectiveOperationException {
			if (mapper!=null) {
				Object valueToSet = mapper.map(propertyValue);
				field.set(instance, valueToSet);
			}
		}
		
		@Override
		public TypeMapper getValueTypeMapper() {
			return mapper;
		}
	}
	
	public List<String> getPropertyNames() {
		return new ArrayList<String>(fieldMappers.keySet());
	}
	
	public TypeMapper getFieldValueMapper(String propertyName) {
		PropertyMapper propertyMapper = fieldMappers.get(propertyName);
		if (propertyMapper != null) {
			return propertyMapper.getValueTypeMapper();
		}
		return null;
	}

	static class MethodMapper implements PropertyMapper, Logger {
		
		private final TypeMapper<?> mapper;
		private final Method method;
		
		public MethodMapper(TypeMapper<?> mapper, Method method) {
			this.mapper = mapper;
			this.method = method;
			method.setAccessible(true);
		}
		
		@Override
		public void map(Object instance, JsonValue propertyValue) throws ReflectiveOperationException {
			if (mapper!=null) {
				Object valueToSet = mapper.map(propertyValue);
				try {
					method.invoke(instance, valueToSet);
				} catch(ReflectiveOperationException | IllegalArgumentException e) {
					System.err.println("problems with setting value using method:"+method+", value="+valueToSet+", mapper="+mapper);
					throw e;
				}
			}
		}
		
		@Override
		public TypeMapper getValueTypeMapper() {
			return mapper;
		}
	}
	
	public ObjectTypeMapper(TypeMapperRegistry mapperRegistry, Type type) {
		this.mapperRegistry = mapperRegistry;
		this.type = type;
	}


	@Override
	public void addNotify() {
		if (type instanceof Class) {
			Class<?> cls = (Class<?>) type;
			try {
				constructor = cls.getConstructor();
				
				
				Collection<Method> declaredMethods = collectMethods(cls);
//				Method[] declaredMethods = cls.getDeclaredMethods();
				for(Method method : declaredMethods) {
					String methodName = method.getName();
					if (!methodName.startsWith("set") && methodName.length()>3) {
						continue;
					}
					if (method.getParameterCount()!=1) {
						continue;
					}
					
					String name = "" + Character.toLowerCase(methodName.charAt(3)) + methodName.substring(4);
					AnnotatedType annotatedType = method.getAnnotatedParameterTypes()[0];
					TypeMapper<?> mapper = mapperRegistry.getMapper(annotatedType.getType());
					fieldMappers.computeIfAbsent(name, (n) -> new MethodMapper(mapper, method));
					
				}
				
				
				Field[] declaredFields = cls.getDeclaredFields();
				for(Field field : declaredFields) {
					String name = field.getName();
					AnnotatedType annotatedType = field.getAnnotatedType();
					TypeMapper<?> mapper = mapperRegistry.getMapper(annotatedType.getType());
					fieldMappers.computeIfAbsent(name, (n) -> new FieldMapper(mapper, field));
				}
			} catch (ReflectiveOperationException e) {
				throw new RuntimeException(e);
			}
		}

	}
	
	
	private Collection<Method> collectMethods(Class<?> cls) {
		Map<String, Method> map = new HashMap<String, Method>();
		while(cls !=null) {
			Method[] declaredMethods = cls.getDeclaredMethods();
			for(Method method : declaredMethods) {
				map.putIfAbsent(method.getName(), method);
			}
			cls = cls.getSuperclass();
		}
		
		return map.values();
	}


	@Override
	public Object map(JsonValue value) {
		if (value instanceof JsonObject) {
			return mapTyped((JsonObject) value);
		}
		return null;
	}

	private Object mapTyped(JsonObject value) {
		Object result = null;
		try {
			if (type instanceof Class) {
				Class<?> cls = (Class<?>) type;
				result = constructor.newInstance();
				
				
				for(JsonField jsonField : value) {
					String name = jsonField.name();
					PropertyMapper propertyMapper = fieldMappers.get(name);
					if (propertyMapper==null) {
						JsonError error = new JsonError(JsonError.Type.MODEL, Level.WARN, "Unknown property");
						jsonField.getKey().addError(error );
						log("warn no mapper for:"+name);
					} else {
						propertyMapper.map(result, jsonField.getValue());
					}
				}
			}
			
		} catch (ReflectiveOperationException e) {
			e.printStackTrace();
		}
		
		value.setOutput(result);
		JsonNode jsonMostLeftNode = value.getChildren().get(0);
		log("jsonMostLeftNode="+jsonMostLeftNode);
		jsonMostLeftNode.setOutput(result);
		
		return result;
	}
}
