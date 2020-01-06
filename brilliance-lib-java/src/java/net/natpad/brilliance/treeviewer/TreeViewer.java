package net.natpad.brilliance.treeviewer;

import net.natpad.brilliance.gtk.GtkWidget;

public class TreeViewer extends GtkWidget {

	public TreeViewer() {
		super(TreeViewerNative.createTreeViewer());
	}
	
}
