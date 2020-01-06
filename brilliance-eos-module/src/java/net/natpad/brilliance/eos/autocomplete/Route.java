package net.natpad.brilliance.eos.autocomplete;

import java.util.Map;

public interface Route {

	String name();
	
	TypeDescription type();
	
	Map<String, Route> children();
	
}
