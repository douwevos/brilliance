package net.natpad.brilliance.chameleon;

import net.natpad.brilliance.chameleon.DocumentListenerHooks.Hook;
import net.natpad.brilliance.gtk.GtkWidget;

public class Chameleon {

	static {
        System.loadLibrary("brilliancejni");
    }
	
	public static native long createEditor(Document document);

	public static native long createDocument(DocumentManager manager);

	public static native void documentHookListener(long nativeId, Hook hook);

	public static native long createRevisionReader(Revision revision);

	public static native int revisionReaderScanUtf8Char(RevisionReader revisionReader);

	public static native long createDocumentManager();
	
	public static native long createDocumentView();
	public static native void constructDocumentView(DocumentView documentView, Document document, long pango_context, GtkWidget widget);
	public static native void documentViewSetPlainSelection(DocumentView documentView, Cursor startCursor,
			Cursor filterCursor);

	public static native DocumentView editorGetDocumentView(ChameleonEditor chameleonEditor);

	public static native void editorPostUowInsertText(ChameleonEditor chameleonEditor, String text);

	public static native Revision documentGetCurrentRevision(Document document);

	public static native void pageHoldLines(Page page);

	public static native void pageReleaseLines(Page page);

	public static native Line pageLineAt(Page page, int pageLineIndex);

	public static native String lineGetText(Line line);

	public static native boolean editorGotoLine(ChameleonEditor chameleonEditor, long line);

	public static native void documentManagerLoad(DocumentManager documentManager, Document document, String path);
	
}
