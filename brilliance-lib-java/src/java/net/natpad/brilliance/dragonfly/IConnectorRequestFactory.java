package net.natpad.brilliance.dragonfly;

import net.natpad.brilliance.caterpillar.StringWo;
import net.natpad.brilliance.chameleon.Document;
import net.natpad.brilliance.chameleon.Revision;

public interface IConnectorRequestFactory {

	AugmentRequest createRequest(Document document, Revision revision);
	
	StringWo getSlotKey();
	
}
