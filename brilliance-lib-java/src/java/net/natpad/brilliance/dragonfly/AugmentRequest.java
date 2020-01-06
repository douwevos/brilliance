package net.natpad.brilliance.dragonfly;

import net.natpad.brilliance.Logger;
import net.natpad.brilliance.caterpillar.StringWo;
import net.natpad.brilliance.chameleon.Document;
import net.natpad.brilliance.chameleon.Revision;
import net.natpad.brilliance.worm.Request;

public abstract class AugmentRequest extends Request implements Logger {

	public AugmentRequest(Document document, Revision revision, StringWo markupSlotKey) {
		super(Dragonfly.createAugmentRequest(document, revision, markupSlotKey));
		Dragonfly.augmentRequestLink(this);
	}
	
	@Override
	protected void finalize() throws Throwable {
		Dragonfly.augmentRequestUnlink(this);
		super.finalize();
	}
	
	@Override
	public final void runRequest() {
		// not called
	}
	
	private boolean runAugmentLink(long revisionId, long keywordPrinterId, long lineTagPrinterId) {
		log("invoke runAugmentLink");
		boolean runAugment = runAugment(new Revision(revisionId), new KeywordPrinter(keywordPrinterId), new KeywordPrinter(lineTagPrinterId));
		log("invoke runAugmentLink done");
		Dragonfly.augmentRequestUnlink(this);
		return runAugment;
	}
	
	public abstract boolean runAugment(Revision revision, KeywordPrinter keywordPrinter, KeywordPrinter line_tag_printer);

	
}
