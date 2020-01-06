package net.natpad.brilliance;

public interface Logger {

	public static final LoggerLevel defaultLevel = LoggerLevel.INFO;
	
	default void log(String text) {
		Exception e = new Exception();
		StackTraceElement[] stackTrace = e.getStackTrace();
		
		
		try {
			Class<?> loggerClass = getClass().getClassLoader().loadClass(stackTrace[1].getClassName());
			LoggerConfig annotation = loggerClass.getAnnotation(LoggerConfig.class);
			if (annotation==null) {
				return;
			}
		} catch (ClassNotFoundException e1) {
		}
		
		String s = stackTrace[1].getClassName()+"."+stackTrace[1].getMethodName() + "(" + stackTrace[1].getLineNumber() + "):";
		System.out.println("## "+s+text);
	}
	
}
