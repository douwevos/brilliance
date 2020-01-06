package net.natpad.brilliance.dragonfly;

import net.natpad.brilliance.caterpillar.StringWo;
import net.natpad.brilliance.chameleon.ChameleonEditor;
import net.natpad.brilliance.chameleon.Cursor;
import net.natpad.brilliance.chameleon.Document;
import net.natpad.brilliance.chameleon.Revision;
import net.natpad.brilliance.chameleon.RevisionReader;
import net.natpad.brilliance.gtk.GtkWidget;
import net.natpad.brilliance.leafhopper.Frame;
import net.natpad.brilliance.leafhopper.IPanelOwner;
import net.natpad.brilliance.worm.WorkerService;

public class Dragonfly {

	static {
        System.loadLibrary("brilliancejni");
    }

	public static native long createEditor();

	public static native long createConnectorMap(WorkerService workerService);

	public static native long createKeywordPrinter(Revision revision, StringWo markupKey, int markupIdx);

	public static native long createAugmentRequest(Document document, Revision revision, StringWo markupSlotKey);

	public static native void augmentRequestLink(AugmentRequest augmentRequest);

	public static native void augmentRequestUnlink(AugmentRequest augmentRequest);

	public static native void keyWordPrinterPrintFgColor(KeywordPrinter printer, int beginRow, int beginColumn, int endRow,
			int endColumn, int colorIndex);

	public static native long createEditorPanel();

	public static native long createPanelOwner(Frame frame, WorkerService workerService);

	public static native long editorPanelGetEditor(EditorPanel panel);

	public static native long createContextEditor(ContextEditor contextEditor, EditorPanel panel);

	public static native void editorSetContextEditor(DragonflyEditor editor, ContextEditor contextEditor);

	public static native RevisionReader acContextCreateRevisionReader(AcContext acContext);

	public static native Cursor acContextGetEndCursor(AcContext acContext);
	public static native Cursor acContextGetStartCursor(AcContext acContext);
	public static native Cursor acContextGetFilterCursor(AcContext acContext);

	public static native Revision acContextGetRevision(AcContext acContext);

	public static native long createLineTag(long row, LineTagType tagType, LineTagLevel tagLevel);

	public static native void keyWordPrinterPrintLineTag(KeywordPrinter keywordPrinter, LineTag tag);

	public static native void lineTagSetText(LineTag lineTag, String text);

	public static native void lineTagSetStartAndEndIndex(LineTag lineTag, int start, int end);

	public static native void lineTagSetColor(LineTag lineTag, double red, double green, double blue);

	public static native long createAcEntry(String text);

	public static native void acContextAddEntry(AcContext acContext, AcEntry entry);

	public static native void acEntryLink(AcEntry acEntry);

	public static native void acEntryUnlink(AcEntry acEntry);

	public static native ChameleonEditor acContextGetEditor(AcContext acContext);

	public static native void editorLink(DragonflyEditor dragonflyEditor);

	public static native void editorPanelLink(EditorPanel editorPanel);

	public static native void constructEditorPanelLink(EditorPanel editorPanel, IPanelOwner panelOwner, Document document,
			IConnectorRequestFactory connectorFactory);

	public static native void constructEditor(DragonflyEditor dragonflyEditor, Document document, ConnectorMap connectorMap,
			IConnectorRequestFactory connectorFactory, WorkerService workerService);
	public static native long editorGetLineInfoKey(DragonflyEditor dragonflyEditor);

	public static native long createDocumentView();

	public static native void constructDocumentView(DocumentView documentView, Document document, long pango_context,
			GtkWidget widget, long info_key);

	public static native void editorUowInsertTemplateRaw(DragonflyEditor dragonflyEditor, String text);

	public static native long constructAcEntryFull(String text, String typeText, int colorCode, int typeColorCode,
			boolean enabled);

}
