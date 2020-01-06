package net.natpad.json;

public class JsonError {

	private final Type type;
	private final Level level;
	
	private final String message;
	
	public JsonError(Type type, Level level, String message) {
		this.type = type;
		this.level = level;
		this.message = message;
	}
	
	public Type getType() {
		return type;
	}
	
	public Level getLevel() {
		return level;
	}
	
	public String getMessage() {
		return message;
	}
	
	public static enum Type {
		SCANNER,
		PARSER,
		MODEL
	}
	
	public static enum Level {
		ERROR,
		WARN,
		UNSAFE,
		INFO
	}
	
}
