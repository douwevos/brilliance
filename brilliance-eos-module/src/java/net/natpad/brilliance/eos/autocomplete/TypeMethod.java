package net.natpad.brilliance.eos.autocomplete;

import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.concurrent.CopyOnWriteArrayList;

public class TypeMethod implements Route {

	private final TypeDescription returnType;
	private final String methodName;
	private final List<TypeField> arguments;
	
	public TypeMethod(TypeDescription returnType, String methodName, List<TypeField> arguments) {
		this.returnType = returnType;
		this.methodName = methodName;
		this.arguments = new CopyOnWriteArrayList<TypeField>(arguments);
	}

	@Override
	public String name() {
		return methodName;
	}

	@Override
	public TypeDescription type() {
		return returnType;
	}
	
	public List<TypeField> getArguments() {
		return new CopyOnWriteArrayList<TypeField>(arguments);
	}
	
	@Override
	public Map<String, Route> children() {
		Map<String, Route> result = new HashMap<String, Route>();
		returnType.fields().stream().forEach(p -> result.put(p.name(), p));
		return result;
	}

}
