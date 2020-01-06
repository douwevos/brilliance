package net.natpad.brilliance.eos;

import java.io.File;
import java.io.IOException;
import java.io.StringReader;
import java.util.ArrayList;
import java.util.List;

import net.natpad.brilliance.Logger;
import net.natpad.brilliance.caterpillar.Caterpillar;
import net.natpad.brilliance.chameleon.Document;
import net.natpad.brilliance.chameleon.DocumentManager;
import net.natpad.brilliance.dragonfly.ContextEditor;
import net.natpad.brilliance.dragonfly.DragonflyEditor;
import net.natpad.brilliance.dragonfly.IConnectorRequestFactory;
import net.natpad.brilliance.dragonfly.PanelOwner;
import net.natpad.brilliance.gtk.GtkMain;
import net.natpad.brilliance.gtk.GtkWindow;
import net.natpad.brilliance.leafhopper.Frame;
import net.natpad.brilliance.leafhopper.Surface;
import net.natpad.brilliance.leafhopper.SurfaceBox;
import net.natpad.brilliance.leafhopper.SurfaceHandler;
import net.natpad.brilliance.leafhopper.SurfaceTab;
import net.natpad.brilliance.worm.Worm;
import net.natpad.json.JsonScanner;
import net.natpad.json.JsonToken;
import net.natpad.json.JsonTokenType;

public class Main implements Logger {

	public static void main(String[] args) {
		Main main = new Main();
		main.start();
		main = null;
//		log("system.gc called");
		System.gc();
		try {
			Thread.sleep(3000);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		Caterpillar.referenceReport();
	}
	
	public Main() {
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

		
		Document document = new Document(documentManager);

//		GtkScrolledWindow scrolledWindow2 = new GtkScrolledWindow();
		
//		ConnectorMap connectorMap = new ConnectorMap(Worm.getWorkerService());
		

		IConnectorRequestFactory factory = new EosConnectorFactory();
//		DragonflyEditor editor2 = new DragonflyEditor(document, connectorMap, factory);
		

		PanelOwner panelOwner = new PanelOwner(frame, Worm.getWorkerService());
		log("panelOwner created");
		EditorPanel panel = new EditorPanel(panelOwner, document, factory);
		log("EditorPanel created");
		
		DragonflyEditor editor = panel.getEditor();
		ContextEditor contextEditor = new EosContextEditor(panel);
		editor.setContextEditor(contextEditor);
		log("contextEditor was set");

		
//		scrolledWindow2.add(panel);
		log("panel added");
		SurfaceTab tab2 = new SurfaceTab(panel, "Editor2");
		surface.getTabModel().addTab(tab2);
		
		
		window.showAll();
		window.setSizeRequest(1040, 350);
		
		log("Show all");
		
		File file = new File("/home/dvos/workspaces/openshop/eos-ts-openshop/etc/config/pm/rvl_module.json");
		documentManager.load(document, file);
		panel.setName(file.getName());
		
		GtkMain.main();
	
		log("###################################");
		
		try {
			Thread.sleep(2000L);
		} catch (InterruptedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}


	public static class JsonTokenizedLine {
		
		private final String line;
		private final List<JsonToken> tokens = new ArrayList<>();
		
		public JsonTokenizedLine(String line) {
			this.line = line;
			try {
				JsonScanner scanner = new JsonScanner(new StringReader(line));
				while(true) {
					JsonToken token = scanner.next();
					if (token.getType() == JsonTokenType.EOT) {
						break;
					}
					tokens.add(token);
				}
			} catch (IOException e) {
			}
		}
		
		
		public JsonToken get(int tokenIdx) {
			return tokens.get(tokenIdx);
		}


		public int indexOf(int utf8offset) {
			int found = -1;
			for(int idx=0; idx<tokens.size(); idx++) {
				JsonToken jsonToken = tokens.get(idx);
				if ((utf8offset>=jsonToken.getLeft().getUtf8offset())  && 
						(utf8offset<=jsonToken.getRight().getUtf8offset())) {
					found = idx;
					break;
				}
			}
			return found;
		}
		
	}

}
