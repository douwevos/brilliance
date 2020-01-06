package net.natpad.brilliance;

import net.natpad.brilliance.chameleon.DocumentManager;
import net.natpad.brilliance.gtk.GtkMain;
import net.natpad.brilliance.gtk.GtkWindow;
import net.natpad.brilliance.leafhopper.Frame;
import net.natpad.brilliance.leafhopper.Surface;
import net.natpad.brilliance.leafhopper.SurfaceBox;
import net.natpad.brilliance.leafhopper.SurfaceHandler;
import net.natpad.brilliance.worm.Worm;

public class BrillianceMain implements Logger {

	public static void main(String[] args) {
		BrillianceMain brillianceMain = new BrillianceMain();
		brillianceMain.start();
	}
	
	public BrillianceMain() {
	}
	
	// /usr/lib/jvm/oracle-java8-jdk-amd64/jre/lib/amd64/:/usr/lib/jvm/oracle-java8-jdk-amd64/jre/lib/amd64/server/:/home/dvos/cpp-workspace/natpad/natpad-lib/dung/b977ce62f98158b2/

	
	private void start() {
		GtkWindow window = new GtkWindow();
		Frame frame = new Frame(window);
		window.add(frame);
//		
		
		Worm.setupWormService();
		
		DocumentManager documentManager = new DocumentManager();
		
		
		SurfaceHandler surfaceHandler = frame.getSurfaceHandler();
		SurfaceBox root = surfaceHandler.getRoot();
		
		Surface surface = new Surface();
		frame.add(surface);
		root.add(surface, 1, 200);

//		
//		Document document = new Document(documentManager);
//
//		GtkScrolledWindow scrolledWindow2 = new GtkScrolledWindow();
//		
////		ConnectorMap connectorMap = new ConnectorMap(Worm.getWorkerService());
//		
//
//		IConnectorRequestFactory factory = new MyFirstConnectorFactory();
////		DragonflyEditor editor2 = new DragonflyEditor(document, connectorMap, factory);
//		
//
//		PanelOwner panelOwner = new PanelOwner(frame, Worm.getWorkerService());
//		log("panelOwner created");
//		EditorPanel panel = new EditorPanel(panelOwner , document, factory);
//		log("EditorPanel created");
//		
//		DragonflyEditor editor = panel.getEditor();
////		ContextEditor contextEditor = new MyContextEditor(panel);
////		editor.setContextEditor(contextEditor);
//		log("contextEditor was set");
//
//		
////		scrolledWindow2.add(panel);
//		log("panel added");
//		SurfaceTab tab2 = new SurfaceTab(panel, "Editor2");
//		surface.getTabModel().addTab(tab2);


		log("adding treeviewer");

//		TreeViewer treeViewer = new TreeViewer();
//		SurfaceTab tab3 = new SurfaceTab(treeViewer, "Tree viewer");
//		surface.getTabModel().addTab(tab3);
		log("added treeviewer");
		
		
		
		window.setSizeRequest(1040, 350);
		window.showAll();
		
		log("Show all");
		GtkMain.main();
	
		log("###################################");
		
		try {
			Thread.sleep(50000L);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	
}
