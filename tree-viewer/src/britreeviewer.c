/*
 * britreeviewer.c
 *
 *  Created on: Sep 28, 2019
 *      Author: dvos
 */

#include "britreeviewer.h"
#include "layout/britreelayout.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_ALL
#define CAT_LOG_CLAZZ "BriTreeViewer"
#include <logging/catlog.h>

struct _BriTreeViewerPrivate {
	BriTreeModel *model;
	BriTreeLayout *layout;
};

G_DEFINE_TYPE_WITH_CODE(BriTreeViewer, bri_tree_viewer, GTK_TYPE_DRAWING_AREA,  // @suppress("Unused static function")
	G_ADD_PRIVATE(BriTreeViewer)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static gboolean l_draw(GtkWidget *widget, cairo_t *cr, gpointer view_as_ptr);
static gboolean l_button_press_event(GtkWidget *gwidget, GdkEventButton *eev, gpointer user_data);

static void bri_tree_viewer_class_init(BriTreeViewerClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void bri_tree_viewer_init(BriTreeViewer *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	BriTreeViewer *instance = BRI_TREE_VIEWER(object);
//	BriTreeViewerPrivate *priv = bri_tree_viewer_get_instance_private(instance);
	G_OBJECT_CLASS(bri_tree_viewer_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(bri_tree_viewer_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


BriTreeViewer *bri_tree_viewer_new(BriTreeModel *model) {
	BriTreeViewer *result = g_object_new(BRI_TYPE_TREE_VIEWER, NULL);
	cat_ref_anounce(result);
	BriTreeViewerPrivate *priv = bri_tree_viewer_get_instance_private(result);
	priv->model = cat_ref_ptr(model);

	priv->layout = bri_tree_layout_new(model);

	GtkWidget *main_widget = GTK_WIDGET(result);

	gtk_widget_set_can_focus(main_widget, TRUE);
	gtk_widget_set_app_paintable(main_widget, TRUE);

	gtk_widget_set_events (main_widget, gtk_widget_get_events (main_widget)
	                                             | GDK_BUTTON_PRESS_MASK
	                                             | GDK_BUTTON2_MASK
	                                             | GDK_BUTTON3_MASK
	                                             | GDK_BUTTON4_MASK
	                                             | GDK_BUTTON5_MASK
	                                             | GDK_BUTTON_RELEASE_MASK
	                                             | GDK_POINTER_MOTION_MASK
	                                             | GDK_POINTER_MOTION_HINT_MASK
	                                             | GDK_BUTTON_RELEASE_MASK
	                                             | GDK_SCROLL_MASK
	                                             );

	g_signal_connect(result, "draw", G_CALLBACK(l_draw), result);
	g_signal_connect(result, "button-press-event", G_CALLBACK(l_button_press_event), result);

	return result;
}




static gboolean l_draw(GtkWidget *widget, cairo_t *cairo, gpointer view_as_ptr) {
	BriTreeViewer *tree_viewer = BRI_TREE_VIEWER(view_as_ptr);
	BriTreeViewerPrivate *priv = bri_tree_viewer_get_instance_private(tree_viewer);

	PangoContext *pango_context = gtk_widget_create_pango_context(widget);


	bri_tree_layout_validate(priv->layout, pango_context);

	cairo_set_source_rgb(cairo, 1.,0.,0.);

	bri_tree_layout_draw(priv->layout, pango_context, cairo);

	return TRUE;
}

static void l_validate_and_paint(BriTreeViewer *tree_viewer) {
	BriTreeViewerPrivate *priv = bri_tree_viewer_get_instance_private(tree_viewer);

//	cairo_t *cairo = gdk_cairo_create(gtk_widget_get_window((GtkWidget *) tree_viewer));
//	PangoContext *pango_context = gtk_widget_create_pango_context((GtkWidget *) tree_viewer);

//	moo_layout_model_validate(priv->layout_model, cairo, pango_context);
//
//	int cur_width, cur_height, view_width, view_height;
//	gtk_widget_get_size_request((GtkWidget *) tree_viewer, &cur_width, &cur_height);
//	moo_layout_model_get_view_size(priv->layout_model, &view_width, &view_height);
//	cat_log_debug("current-size=[%d,%d], view-size=[%d, %d]", cur_width, cur_height, view_width, view_height);

	GtkWidget *s_window = gtk_widget_get_parent((GtkWidget *) tree_viewer);
	gdk_window_invalidate_rect(gtk_widget_get_window((GtkWidget *) tree_viewer), NULL, FALSE);

//	cairo_destroy(cairo);
}

static void l_get_mouse_location(BriTreeViewer *tree_viewer, int *wmx, int *wmy) {
	int tx;
	int ty;

	GtkWidget *widget = GTK_WIDGET(tree_viewer);
	GdkDeviceManager *device_manager = gdk_display_get_device_manager(gtk_widget_get_display(widget));
	GdkDevice *pointer = gdk_device_manager_get_client_pointer(device_manager);
	gdk_window_get_device_position(gtk_widget_get_window(widget), pointer, &tx, &ty, NULL);

	GtkAllocation allocation;
	gtk_widget_get_allocation(widget, &allocation);

	*wmx = tx+allocation.x;
	*wmy = ty+allocation.y;
	cat_log_debug("wmx:%d, y:%d ", *wmx, *wmy);
}

static gboolean l_button_press_event(GtkWidget *gwidget, GdkEventButton *eev, gpointer user_data) {
	BriTreeViewer *tree_viewer = BRI_TREE_VIEWER(user_data);
	BriTreeViewerPrivate *priv = bri_tree_viewer_get_instance_private(tree_viewer);
	gtk_widget_grab_focus(gwidget);

	gboolean shiftPressed = eev->state & GDK_SHIFT_MASK;
	gboolean ctrlPressed = eev->state & GDK_CONTROL_MASK;

	int xmouse, ymouse;
	l_get_mouse_location(tree_viewer, &xmouse, &ymouse);
	BriTreeLayoutItem *item = bri_tree_layout_model_node_at(priv->layout, xmouse, ymouse);

	if (item!=NULL) {
		if (bri_tree_layout_select(priv->layout, item, TRUE, ctrlPressed, shiftPressed)) {
			l_validate_and_paint(tree_viewer);
		}
//		MooNodeLayoutZone zone = moo_node_layout_get_zone(item, xmouse, ymouse);
//		if (zone==MOO_NODE_LAYOUT_ZONE_EXPANDER) {
//			tryToggleFirst = TRUE;
//		}
//		if (eev->type==GDK_2BUTTON_PRESS) {
//			MooExplorerClass *me_class = MOO_EXPLORER_GET_CLASS(explorer);
//			if (me_class->activateNode) {
//				me_class->activateNode(explorer, item);
//			}
//		} else {
//			if (moo_layout_model_select(priv->layout_model, item, tryToggleFirst, ctrlPressed, shiftPressed)) {
//				l_validate_and_paint(explorer);
//			}
//		}
	}


	return TRUE;
}

