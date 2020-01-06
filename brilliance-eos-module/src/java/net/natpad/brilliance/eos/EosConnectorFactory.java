package net.natpad.brilliance.eos;

import net.natpad.brilliance.caterpillar.StringWo;
import net.natpad.brilliance.chameleon.Document;
import net.natpad.brilliance.chameleon.Revision;
import net.natpad.brilliance.dragonfly.AugmentRequest;
import net.natpad.brilliance.dragonfly.IConnectorRequestFactory;
import net.natpad.brilliance.eos.augment.ModuleAugmentRequest;

public class EosConnectorFactory implements IConnectorRequestFactory {

	private final StringWo slotKey;

	public EosConnectorFactory() {
		slotKey = new StringWo();
	}

	@Override
	public AugmentRequest createRequest(Document document, Revision revision) {
		return new ModuleAugmentRequest(document, revision, slotKey);
	}

	@Override
	public StringWo getSlotKey() {
		return slotKey;
	}
}
