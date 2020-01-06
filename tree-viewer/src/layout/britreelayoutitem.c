/*
   File:    britreelayoutitem.c
   Project: tree-viewer
   Author:  Douwe Vos
   Date:    Oct 3, 2019
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

#include "britreelayoutitem.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_ALL
#define CAT_LOG_CLAZZ "BriTreeLayoutItem"
#include <logging/catlog.h>

struct _BriTreeLayoutItemPrivate {
	BriTreeNodeRef *node;
	CatArrayWo *children;
	int level;
	int view_x;
	int view_width_expander;
	int view_width_icon;
	int view_width_text;
	int lastSelfVersion;
	int lastSubVersion;
};

static void l_init_istringable(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(BriTreeLayoutItem, bri_tree_layout_item, G_TYPE_OBJECT,  // @suppress("Unused static function")
	G_ADD_PRIVATE(BriTreeLayoutItem)
	G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_init_istringable)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void bri_tree_layout_item_class_init(BriTreeLayoutItemClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void bri_tree_layout_item_init(BriTreeLayoutItem *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	BriTreeLayoutItem *instance = BRI_TREE_LAYOUT_ITEM(object);
//	BriTreeLayoutItemPrivate *priv = bri_tree_layout_item_get_instance_private(instance);
	G_OBJECT_CLASS(bri_tree_layout_item_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(bri_tree_layout_item_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


BriTreeLayoutItem *bri_tree_layout_item_new(BriTreeNodeRef *node) {
	BriTreeLayoutItem *result = g_object_new(BRI_TYPE_TREE_LAYOUT_ITEM, NULL);
	cat_ref_anounce(result);
	BriTreeLayoutItemPrivate *priv = bri_tree_layout_item_get_instance_private(result);
	priv->node = cat_ref_ptr(node);
	priv->level = 0;
	priv->children = NULL;
	return result;
}

BriTreeNodeRef *bri_tree_layout_item_get_node_ref(BriTreeLayoutItem *layout_item) {
	BriTreeLayoutItemPrivate *priv = bri_tree_layout_item_get_instance_private(layout_item);
	return priv->node;
}

void bri_tree_layout_item_set_expanded(BriTreeLayoutItem *layout_item, gboolean expanded) {
	BriTreeLayoutItemPrivate *priv = bri_tree_layout_item_get_instance_private(layout_item);
	cat_log_error("node=%p", priv->node);
	bri_tree_node_ref_set_opened(priv->node, expanded);
}

gboolean bri_tree_layout_item_is_expanded(BriTreeLayoutItem *layout_item) {
	BriTreeLayoutItemPrivate *priv = bri_tree_layout_item_get_instance_private(layout_item);
	return bri_tree_node_ref_is_opened(priv->node);
}

void bri_tree_layout_item_set_children(BriTreeLayoutItem *layout_item, CatArrayWo *children) {
	BriTreeLayoutItemPrivate *priv = bri_tree_layout_item_get_instance_private(layout_item);
	cat_ref_swap(priv->children , children);
}

CatArrayWo *bri_tree_layout_item_get_children(BriTreeLayoutItem *layout_item) {
	BriTreeLayoutItemPrivate *priv = bri_tree_layout_item_get_instance_private(layout_item);
	return priv->children;
}

CatStringWo *bri_tree_layout_item_get_text(BriTreeLayoutItem *layout_item) {
	BriTreeLayoutItemPrivate *priv = bri_tree_layout_item_get_instance_private(layout_item);
	return bri_tree_node_ref_get_text(priv->node);
}

gboolean bri_tree_layout_item_can_expand(BriTreeLayoutItem *layout_item) {
	BriTreeLayoutItemPrivate *priv = bri_tree_layout_item_get_instance_private(layout_item);
	return bri_tree_node_ref_can_open(priv->node);
}

void bri_tree_layout_item_set_level(BriTreeLayoutItem *layout_item, int level) {
	BriTreeLayoutItemPrivate *priv = bri_tree_layout_item_get_instance_private(layout_item);
	priv->level = level;
}

int bri_tree_layout_item_get_level(BriTreeLayoutItem *layout_item) {
	BriTreeLayoutItemPrivate *priv = bri_tree_layout_item_get_instance_private(layout_item);
	return priv->level;
}


static void l_print(CatIStringable *self, struct _CatStringWo *append_to) {
	BriTreeLayoutItem *instance = BRI_TREE_LAYOUT_ITEM(self);
	BriTreeLayoutItemPrivate *priv = bri_tree_layout_item_get_instance_private(instance);
	cat_string_wo_format(append_to, "BriTreeLayoutItem[%p, %O]", instance, priv->node);
}


static void l_init_istringable(CatIStringableInterface *iface) {
	iface->print = l_print;
}
