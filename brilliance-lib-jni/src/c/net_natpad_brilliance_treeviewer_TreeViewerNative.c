#include "net_natpad_brilliance_treeviewer_TreeViewerNative.h"
#include <britreeviewer.h>

/*
 * Class:     net_natpad_brilliance_treeviewer_TreeViewerNative
 * Method:    createTreeViewer
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_net_natpad_brilliance_treeviewer_TreeViewerNative_createTreeViewer(JNIEnv *env, jclass cls) {
	BriTreeModel *model = bri_tree_model_new();
	return (jlong) bri_tree_viewer_new(model);
}
