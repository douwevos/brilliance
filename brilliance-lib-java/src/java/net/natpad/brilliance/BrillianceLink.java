//package net.natpad.brilliance;
//
//
//import net.natpad.brilliance.chameleon.ChameleonEditor;
//import net.natpad.brilliance.chameleon.Document;
//import net.natpad.brilliance.chameleon.DocumentManager;
//import net.natpad.brilliance.dragonfly.ConnectorMap;
//import net.natpad.brilliance.dragonfly.DragonflyEditor;
//import net.natpad.brilliance.dragonfly.IConnectorRequestFactory;
//import net.natpad.brilliance.eos.MyFirstConnectorFactory;
//import net.natpad.brilliance.gtk.GtkScrolledWindow;
//import net.natpad.brilliance.gtk.GtkWidget;
//import net.natpad.brilliance.gtk.GtkWindow;
//import net.natpad.brilliance.leafhopper.Frame;
//import net.natpad.brilliance.leafhopper.Surface;
//import net.natpad.brilliance.leafhopper.SurfaceBox;
//import net.natpad.brilliance.leafhopper.SurfaceBox.Orientation;
//import net.natpad.brilliance.leafhopper.SurfaceHandler;
//import net.natpad.brilliance.leafhopper.SurfaceTab;
//import net.natpad.brilliance.treeviewer.TreeViewer;
//import net.natpad.brilliance.worm.Worm;
//
//public class BrillianceLink implements Runnable {
//
//	public static long frameId;
//	public static long documentManagerId;
//	public static long workerServiceId;
//
//	
//	static Frame frame; 
//	static ChameleonEditor editor;
//	static Document document;
//	static GtkScrolledWindow scrolledWindow;
//	
//	public static void main(String[] args) {
//		log("BrillianceLink started");
//		
//		
//		log("Worm="+Worm.getWorkerService());
////		Worm.setupWormService(workerServiceId);
//		
//		frame = new Frame(frameId);
//		DocumentManager documentManager = new DocumentManager(documentManagerId);
//		SurfaceHandler surfaceHandler = frame.getSurfaceHandler();
//		SurfaceBox root = surfaceHandler.getRoot();
//		
////		SurfaceBox s = new SurfaceBox(Orientation.HORIZONTAL);
//		
//		Surface surface = new Surface();
//		frame.add(surface);
//		root.add(surface, 1, 200);
//
//		
//		document = new Document(documentManager);
//
//		//		editor = new ChameleonEditor(document);
////
////		document.addListener(new IDocumentListener() {
////			
////			@Override
////			public void onNewRevision(Revision revision) {
////				log("new revision: revision="+revision);
////				RevisionReader reader = new RevisionReader(revision);
////				log("reader="+reader);
////				StringBuilder buf = new StringBuilder();
////				for(int idx=0; idx<100; idx++) {
////					int ch = reader.scanUtf8Char();
////					if (ch==-1) {
////						break;
////					}
////					buf.appendCodePoint(ch);
////				}
////				log(""+buf);
////				
////			}
////		});
////		
////		scrolledWindow = new GtkScrolledWindow();
////		scrolledWindow.add(editor);
////		
////		SurfaceTab tab = new SurfaceTab(scrolledWindow, "Editor");
////		surface.getTabModel().addTab(tab);
//
//		
//
//		GtkScrolledWindow scrolledWindow2 = new GtkScrolledWindow();
//		
//		ConnectorMap connectorMap = new ConnectorMap(Worm.getWorkerService());
//		
//
//		IConnectorRequestFactory factory = new MyFirstConnectorFactory();
//		DragonflyEditor editor2 = new DragonflyEditor(document, connectorMap, factory);
//		scrolledWindow2.add(editor2);
//		SurfaceTab tab2 = new SurfaceTab(scrolledWindow2, "Editor2");
//		surface.getTabModel().addTab(tab2);
//
//
//		TreeViewer treeViewer = new TreeViewer();
//		SurfaceTab tab3 = new SurfaceTab(treeViewer, "Tree viewer");
//		surface.getTabModel().addTab(tab3);
//		
//		
//		
////		SurfaceBox mainSplit = new SurfaceBox(SurfaceBox.Orientation.VERTICAL);
////		
////		root.add(mainSplit, 0, 300);
//		
//
//		GtkWidget parent = frame.getParent();
//		log("frame="+frame+", parent="+parent);
////		parent.showAll();
//		frame.showAll();
//		
//		log("BrillianceLink finished");
//		
//		Thread thread = new Thread(new BrillianceLink());
//		thread.setDaemon(false);
//		thread.start();
//		
//	}
//	
//	
//	
//	@Override
//	public void run() {
//		while(true) {
//			try {
//				Thread.sleep(2000);
//			} catch (InterruptedException e) {
//				// TODO Auto-generated catch block
//				e.printStackTrace();
//			}
//			log("pung");
//			TestRequest testRequest = new TestRequest();
//			Worm.getWorkerService().postRequest(testRequest);
//		}
//	}
//}
