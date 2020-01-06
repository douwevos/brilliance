/*
   File:    britreenoderef.c
   Project: tree-viewer
   Author:  Douwe Vos
   Date:    Oct 1, 2019
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

#include "britreenoderef.h"
#include "britreemodel.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_ALL
#define CAT_LOG_CLAZZ "BriTreeNodeRef"
#include <logging/catlog.h>

struct _BriTreeNodeRefPrivate {
	BriTreeModel *tree_model;
	BriTreeNode *tree_node;
	BriTreeNodeRef *parent_ref;
	gboolean has_opened;
};

static void l_init_istringable(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(BriTreeNodeRef, bri_tree_node_ref, G_TYPE_OBJECT,  // @suppress("Unused static function")
	G_ADD_PRIVATE(BriTreeNodeRef)
	G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_init_istringable)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void bri_tree_node_ref_class_init(BriTreeNodeRefClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void bri_tree_node_ref_init(BriTreeNodeRef *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	BriTreeNodeRef *instance = BRI_TREE_NODE_REF(object);
//	BriTreeNodeRefPrivate *priv = bri_tree_node_ref_get_instance_private(instance);
	bri_tree_node_ref_set_opened(instance, FALSE);
	G_OBJECT_CLASS(bri_tree_node_ref_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(bri_tree_node_ref_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


BriTreeNodeRef *bri_tree_node_ref_new(struct _BriTreeModel *tree_model, BriTreeNode *tree_node, BriTreeNodeRef *parent_ref) {
	BriTreeNodeRef *result = g_object_new(BRI_TYPE_TREE_NODE_REF, NULL);
	cat_ref_anounce(result);
	BriTreeNodeRefPrivate *priv = bri_tree_node_ref_get_instance_private(result);
	priv->tree_model = cat_ref_ptr(tree_model);
	priv->tree_node = cat_ref_ptr(tree_node);
	priv->parent_ref = cat_ref_ptr(parent_ref);
	return result;
}

void bri_tree_node_inform_parents_of_change(BriTreeNodeRef *node) {
	BriTreeNodeRefPrivate *priv = bri_tree_node_ref_get_instance_private(node);
	BriTreeNodeRef *work = priv->parent_ref;
	while(work) {
		priv = bri_tree_node_ref_get_instance_private(work);
		bri_tree_node_increase_sub_version(priv->tree_node);
	}
}

CatArrayWo *bri_tree_node_ref_enlist_children(BriTreeNodeRef *node) {
	BriTreeNodeRefPrivate *priv = bri_tree_node_ref_get_instance_private(node);
	CatArrayWo *result = NULL;
	if (bri_tree_node_is_open(priv->tree_node)) {
		CatArrayWo *children = bri_tree_node_get_children(priv->tree_node);
		if (children) {
			result = cat_array_wo_new_size(cat_array_wo_size(children));
			CatIIterator *iter = cat_array_wo_iterator(children);
			while(cat_iiterator_has_next(iter)) {
				BriTreeNode *child = (BriTreeNode *) cat_iiterator_next(iter);
				BriTreeNodeRef *new_ref = bri_tree_node_ref_new(priv->tree_model, child, node);
				cat_array_wo_append(result, (GObject *) new_ref);
				cat_unref_ptr(new_ref);
			}
		}
	}
	return result;
}

BriTreeNodeVersions bri_tree_node_ref_get_versions(BriTreeNodeRef *node) {
	BriTreeNodeRefPrivate *priv = bri_tree_node_ref_get_instance_private(node);
	return bri_tree_node_get_versions(priv->tree_node);
}

CatStringWo *bri_tree_node_ref_get_text(BriTreeNodeRef *node) {
	BriTreeNodeRefPrivate *priv = bri_tree_node_ref_get_instance_private(node);
	return bri_tree_node_get_text(priv->tree_node);
}

void bri_tree_node_ref_set_text(BriTreeNodeRef *node, CatStringWo *text) {
	BriTreeNodeRefPrivate *priv = bri_tree_node_ref_get_instance_private(node);
	bri_tree_node_set_text(priv->tree_node, text);
}

void bri_tree_node_ref_set_opened(BriTreeNodeRef *node, gboolean opened) {
	BriTreeNodeRefPrivate *priv = bri_tree_node_ref_get_instance_private(node);
	cat_log_error("priv->node=%p", priv->tree_node);
	if (priv->has_opened == opened) {
		return;
	}
	priv->has_opened = opened;
	if (opened) {
		bri_tree_node_open(priv->tree_node);
	} else {
		bri_tree_node_close(priv->tree_node);
	}
}

gboolean bri_tree_node_ref_is_opened(BriTreeNodeRef *node) {
	BriTreeNodeRefPrivate *priv = bri_tree_node_ref_get_instance_private(node);
	return priv->has_opened;
}

gboolean bri_tree_node_ref_can_open(BriTreeNodeRef *node) {
	BriTreeNodeRefPrivate *priv = bri_tree_node_ref_get_instance_private(node);
	return bri_tree_node_can_open(priv->tree_node);
}


static void l_print(CatIStringable *self, struct _CatStringWo *append_to) {
	BriTreeNodeRef *instance = BRI_TREE_NODE_REF(self);
	BriTreeNodeRefPrivate *priv = bri_tree_node_ref_get_instance_private(instance);
	cat_string_wo_format(append_to, "BriTreeNodeRef[%O, has_openend=%d]", priv->tree_node, priv->has_opened);
}


static void l_init_istringable(CatIStringableInterface *iface) {
	iface->print = l_print;
}
