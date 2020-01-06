package net.natpad.brilliance.eos.autocomplete;

import java.util.List;

public interface TypeDescription {

	String typename();

	List<TypeField> fields();
	
	
}
