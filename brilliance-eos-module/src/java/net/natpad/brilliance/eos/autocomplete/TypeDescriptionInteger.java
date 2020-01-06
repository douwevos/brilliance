package net.natpad.brilliance.eos.autocomplete;

import java.util.Collections;
import java.util.List;

public class TypeDescriptionInteger implements TypeDescription {
	
	@Override
	public String typename() {
		return "Integer";
	}
	
	@Override
	public List<TypeField> fields() {
		return Collections.emptyList();
	}
	
}
