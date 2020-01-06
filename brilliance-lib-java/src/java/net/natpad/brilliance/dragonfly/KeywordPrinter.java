package net.natpad.brilliance.dragonfly;

import net.natpad.brilliance.caterpillar.StringWo;
import net.natpad.brilliance.chameleon.Revision;
import net.natpad.brilliance.glibc.GObject;

public class KeywordPrinter extends GObject {

	KeywordPrinter(long nativeId) {
		super(nativeId);
	}
	
	public KeywordPrinter(Revision revision, StringWo markupKey, int markupIdx) {
		super(Dragonfly.createKeywordPrinter(revision, markupKey, markupIdx));
	}
	
	public void printFgColor(int beginRow, int beginColumn, int endRow, int endColumn, int colorIndex) {
		Dragonfly.keyWordPrinterPrintFgColor(this, beginRow, beginColumn, endRow, endColumn, colorIndex);
	}

	public void printLineTag(LineTag tag) {
		Dragonfly.keyWordPrinterPrintLineTag(this, tag);
	}
}
