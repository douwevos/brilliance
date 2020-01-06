/*
 * britreeviewer.h
 *
 *  Created on: Sep 28, 2019
 *      Author: dvos
 */

#ifndef BRITREEVIEWER_H_
#define BRITREEVIEWER_H_

#include "model/britreemodel.h"
#include <caterpillar.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define BRI_TYPE_TREE_VIEWER              (bri_tree_viewer_get_type())
#define BRI_TREE_VIEWER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), bri_tree_viewer_get_type(), BriTreeViewer))
#define BRI_TREE_VIEWER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), BRI_TYPE_TREE_VIEWER, BriTreeViewerClass))
#define BRI_IS_TREE_VIEWER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), BRI_TYPE_TREE_VIEWER))
#define BRI_IS_TREE_VIEWER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), BRI_TYPE_TREE_VIEWER))
#define BRI_TREE_VIEWER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), BRI_TYPE_TREE_VIEWER, BriTreeViewerClass))

struct _BriTreeViewer {
	GtkDrawingArea parent;
};

struct _BriTreeViewerClass {
	GtkDrawingAreaClass parent_class;
};

typedef struct _BriTreeViewer               BriTreeViewer;
typedef struct _BriTreeViewerPrivate        BriTreeViewerPrivate;
typedef struct _BriTreeViewerClass          BriTreeViewerClass;


GType bri_tree_viewer_get_type();

BriTreeViewer *bri_tree_viewer_new(BriTreeModel *model);

G_END_DECLS


#endif /* BRITREEVIEWER_H_ */
