/*
   File:    britreelayout.c
   Project: tree-viewer
   Author:  Douwe Vos
   Date:    Sep 29, 2019
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2019 Douwe Vos.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#include "britreelayout.h"
#include "britreelayoutitem.h"
#include <math.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_ALL
#define CAT_LOG_CLAZZ "BriTreeLayout"
#include <logging/catlog.h>


struct _BriTreeWork {
	BriTreeNodeRef *node_ref;
	BriTreeLayoutItem *layout_item;
	CatArrayWo *children;
	int child_index_next;
};

typedef struct _BriTreeWork BriTreeWork;

struct _BriTreeLayoutPrivate {
	BriTreeModel *model;
	CatArrayWo *linear;
	int entry_height;
	BriTreeLayoutItem *root_item;
	BriTreeWork *stack;
	int stack_max;
};

G_DEFINE_TYPE_WITH_CODE(BriTreeLayout, bri_tree_layout, G_TYPE_OBJECT,  // @suppress("Unused static function")
	G_ADD_PRIVATE(BriTreeLayout)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void bri_tree_layout_class_init(BriTreeLayoutClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void bri_tree_layout_init(BriTreeLayout *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	BriTreeLayout *instance = BRI_TREE_LAYOUT(object);
//	BriTreeLayoutPrivate *priv = bri_tree_layout_get_instance_private(instance);
	G_OBJECT_CLASS(bri_tree_layout_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(bri_tree_layout_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


BriTreeLayout *bri_tree_layout_new(BriTreeModel *model) {
	BriTreeLayout *result = g_object_new(BRI_TYPE_TREE_LAYOUT, NULL);
	cat_ref_anounce(result);
	BriTreeLayoutPrivate *priv = bri_tree_layout_get_instance_private(result);
	priv->entry_height = 20;
	priv->model = cat_ref_ptr(model);
	priv->linear = NULL;
	BriTreeNodeRef *node_ref = bri_tree_model_get_root_ref(model);
	cat_log_error("node_ref=%p", node_ref);
	priv->root_item = bri_tree_layout_item_new(node_ref);
	bri_tree_layout_item_set_expanded(priv->root_item, TRUE);
	priv->stack_max = 20;
	priv->stack = g_malloc(sizeof(BriTreeWork) * (priv->stack_max));
	return result;
}

BriTreeLayoutItem *bri_tree_layout_model_node_at(BriTreeLayout *layout, int xmouse, int ymouse) {
	BriTreeLayout *instance = BRI_TREE_LAYOUT(layout);
	BriTreeLayoutPrivate *priv = bri_tree_layout_get_instance_private(instance);
	BriTreeLayoutItem *result = NULL;
	if (cat_array_wo_size(priv->linear)>0) {
		int offset = ymouse/priv->entry_height;
		if (offset>=0 && cat_array_wo_size(priv->linear)) {
			result = (BriTreeLayoutItem *) cat_array_wo_get(priv->linear, offset);
		}
	}
	return result;

}


static int l_calculate_entity_height(PangoContext *pango_context) {
	PangoFontMetrics *font_metrics = pango_context_get_metrics(pango_context, NULL, NULL);
	double font_height = ((double) (pango_font_metrics_get_ascent(font_metrics) + pango_font_metrics_get_descent(font_metrics)))/PANGO_SCALE;
	return ceil(font_height)+2;
}

static BriTreeLayoutItem *l_find_old(CatArrayWo *old_items, CatStringWo *id) {
	BriTreeLayoutItem *result = NULL;
	CatIIterator *iter = cat_array_wo_iterator(old_items);
	while(cat_iiterator_has_next(iter)) {
		BriTreeLayoutItem *child = (BriTreeLayoutItem *) cat_iiterator_next(iter);
		CatStringWo *child_text = bri_tree_layout_item_get_text(child);
		if (cat_string_wo_equal(child_text, id)) {
			result = child;
			break;
		}
	}
	cat_unref_ptr(iter);
	return result;
}

static void l_sync_children(BriTreeWork *work) {
	BriTreeNodeRef *item_node_ref = bri_tree_layout_item_get_node_ref(work->layout_item);
	CatArrayWo *raw_nodes = bri_tree_node_ref_enlist_children(item_node_ref);
	cat_log_debug("raw_nodes=%O", raw_nodes);

	CatArrayWo *new_children = cat_array_wo_new_size(cat_array_wo_size(raw_nodes));
	CatIIterator *iter = cat_array_wo_iterator(raw_nodes);
	while(cat_iiterator_has_next(iter)) {
		BriTreeNodeRef *child_node_ref = (BriTreeNodeRef *) cat_iiterator_next(iter);
		CatStringWo *id = bri_tree_node_ref_get_text(child_node_ref);
		BriTreeLayoutItem *found = NULL;
		if (work->children) {
			found = l_find_old(work->children, id);
		}
		if (found == NULL) {
			found = bri_tree_layout_item_new(child_node_ref);
			cat_array_wo_append(new_children, (GObject *) found);
			cat_unref_ptr(found);
		} else {
			cat_array_wo_append(new_children, (GObject *) found);
		}
	}
	cat_unref_ptr(iter);

	cat_ref_swap(work->children, new_children);
	bri_tree_layout_item_set_children(work->layout_item, new_children);
}

static void l_start_layout_work(BriTreeWork *work, BriTreeLayoutItem *layout_item, int level) {
	work->layout_item = cat_ref_ptr(layout_item);
	work->child_index_next = 0;
	work->children = NULL;
	bri_tree_layout_item_set_level(layout_item, level);
	BriTreeNodeRef *item_node_ref = bri_tree_layout_item_get_node_ref(layout_item);
	BriTreeNodeVersions versions = bri_tree_node_ref_get_versions(item_node_ref);
	if (bri_tree_layout_item_is_expanded(layout_item)) {
		work->children = bri_tree_layout_item_get_children(layout_item);
		l_sync_children(work);
	}
	cat_log_debug("children=%O", work->children);
}

void bri_tree_layout_validate(BriTreeLayout *layout, PangoContext *pango_context) {
	BriTreeLayoutPrivate *priv = bri_tree_layout_get_instance_private(layout);
	cat_log_debug("-----------------");
	CatArrayWo *new_list = cat_array_wo_new();

	priv->entry_height = l_calculate_entity_height(pango_context);

	BriTreeLayoutItem *item = priv->root_item;

	int stack_index = 0;
	BriTreeWork *work = priv->stack+stack_index;
	l_start_layout_work(work, item, 0);
	cat_array_wo_append(new_list, (GObject *) work->layout_item);

	while(TRUE) {
		BriTreeWork *work = priv->stack+stack_index;
		cat_log_debug("stack_index=%d, child_index_next=%d, %O", stack_index, work->child_index_next, work->layout_item);

		if (work->children) {
			int child_index = work->child_index_next;
			if (child_index>=cat_array_wo_size(work->children)) {
				stack_index--;
				if (stack_index<0) {
					break;
				}
			} else {
				BriTreeLayoutItem *child = cat_array_wo_get(work->children, work->child_index_next);
				work->child_index_next++;
				stack_index++;
				work = priv->stack+stack_index;
				l_start_layout_work(work, child, stack_index);
				cat_array_wo_append(new_list, (GObject *) work->layout_item);
			}

		} else {
			stack_index--;
			if (stack_index<0) {
				break;
			}
		}

	}


//	CatStringWo *text = bri_tree_node_ref_get_text(node_ref);
//	cat_array_wo_append(new_list, (GObject *) text);

	cat_ref_swap(priv->linear, new_list);
}

static void l_draw_expander(cairo_t *cr, gint x, gint y, double expander_size, gboolean is_expanded);


void bri_tree_layout_draw(BriTreeLayout *layout, PangoContext *pango_context, cairo_t *cairo) {
	BriTreeLayoutPrivate *priv = bri_tree_layout_get_instance_private(layout);
	if (priv->linear) {
		int item_height = priv->entry_height;
		int item_height_half = item_height/2;
		CatIIterator *iter = cat_array_wo_iterator(priv->linear);
		int y = 0;
		PangoLayout *pango_layout = pango_layout_new(pango_context);

		while(cat_iiterator_has_next(iter)) {

			BriTreeLayoutItem *item = (BriTreeLayoutItem *) cat_iiterator_next(iter);
			int x = bri_tree_layout_item_get_level(item) * item_height;
			if (bri_tree_layout_item_can_expand(item)) {
				gboolean is_expaned = bri_tree_layout_item_is_expanded(item);
				l_draw_expander(cairo, x+item_height_half, item_height_half+y, item_height, is_expaned);
			}
			CatStringWo *text = bri_tree_layout_item_get_text(item);
			cairo_move_to(cairo, x+item_height_half+item_height, y);
			pango_layout_set_text(pango_layout, cat_string_wo_getchars(text), cat_string_wo_length(text));
			cairo_set_source_rgb(cairo, 0.0, 0.0, 0.0);
			pango_cairo_show_layout(cairo, pango_layout);
			y+=priv->entry_height;
		}
		cat_unref_ptr(iter);

		cat_unref_ptr(pango_layout);
	}
}



static void l_draw_expander(cairo_t *cr, gint x, gint y, double expander_size, gboolean is_expanded) {
	gint line_width;
	double vertical_overshoot;
	int diameter;
	double radius;
	double interp; /* interpolation factor for center position */
	double x_double_horz, y_double_horz;
	double x_double_vert, y_double_vert;
	double x_double, y_double;
	gint degrees = 0;

	cairo_save(cr);


	line_width = MAX (1, expander_size/9);

	if (is_expanded) {
		degrees = 90;
		interp = 1.0;
	} else {
		degrees = 0;
		interp = 0.0;
	}

	/* Compute distance that the stroke extends beyonds the end
	 * of the triangle we draw.
	 */
	vertical_overshoot = line_width / 2.0 * (1. / tan(G_PI / 8));

	/* For odd line widths, we end the vertical line of the triangle
	 * at a half pixel, so we round differently.
	 */
	if (line_width % 2 == 1)
		vertical_overshoot = ceil(0.5 + vertical_overshoot) - 0.5;
	else
		vertical_overshoot = ceil(vertical_overshoot);

	/* Adjust the size of the triangle we draw so that the entire stroke fits
	 */
	diameter = MAX (3, expander_size - 2 * vertical_overshoot);

	/* If the line width is odd, we want the diameter to be even,
	 * and vice versa, so force the sum to be odd. This relationship
	 * makes the point of the triangle look right.
	 */
	diameter -= (1 - (diameter + line_width) % 2);

	radius = diameter / 2.;

	/* Adjust the center so that the stroke is properly aligned with
	 * the pixel grid. The center adjustment is different for the
	 * horizontal and vertical orientations. For intermediate positions
	 * we interpolate between the two.
	 */
	x_double_vert = floor(x - (radius + line_width) / 2.)
			+ (radius + line_width) / 2.;
	y_double_vert = y - 0.5;

	x_double_horz = x - 0.5;
	y_double_horz = floor(y - (radius + line_width) / 2.)
			+ (radius + line_width) / 2.;

	x_double = x_double_vert * (1 - interp) + x_double_horz * interp;
	y_double = y_double_vert * (1 - interp) + y_double_horz * interp;

	cairo_translate(cr, x_double, y_double);
	cairo_rotate(cr, degrees * G_PI / 180);

	cairo_move_to(cr, -radius / 2., -radius);
	cairo_line_to(cr, radius / 2., 0);
	cairo_line_to(cr, -radius / 2., radius);
	cairo_close_path(cr);

	cairo_set_line_width(cr, line_width);

	cairo_set_source_rgb(cr, 0.1875, 0.1875, 0.1875);

	cairo_fill(cr);

//	gdk_cairo_set_source_color(cr, &style->fg[state_type]);
//	cairo_stroke(cr);
	cairo_restore(cr);
}


gboolean bri_tree_layout_select(BriTreeLayout *layout, BriTreeLayoutItem *item, gboolean toggle, gboolean ctrlPressed, gboolean shiftPressed) {
	BriTreeLayoutPrivate *priv = bri_tree_layout_get_instance_private(layout);
	if (bri_tree_layout_item_can_expand(item)) {
		gboolean was_expanded = bri_tree_layout_item_is_expanded(item);
		bri_tree_layout_item_set_expanded(item, !was_expanded);
	}
	return TRUE;
}
