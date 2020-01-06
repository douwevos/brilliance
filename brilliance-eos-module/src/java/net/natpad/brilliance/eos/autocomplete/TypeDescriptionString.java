package net.natpad.brilliance.eos.autocomplete;

import java.util.Collections;
import java.util.List;

public class TypeDescriptionString implements TypeDescription {
	
	@Override
	public String typename() {
		return "String";
	}
	
	@Override
	public List<TypeField> fields() {
		return Collections.emptyList();
	}
	
}
