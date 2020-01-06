/*
   File:    britreenode.c
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

#include "britreenode.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_ALL
#define CAT_LOG_CLAZZ "BriTreeNode"
#include <logging/catlog.h>

struct _BriTreeNodePrivate {
	BriTreeNode *parent_node;
	int openCount;
	CatArrayWo *children;
	CatStringWo *text;

	BriTreeNodeVersions versions;

};

G_DEFINE_TYPE_WITH_CODE(BriTreeNode, bri_tree_node, G_TYPE_OBJECT,  // @suppress("Unused static function")
	G_ADD_PRIVATE(BriTreeNode)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_open(BriTreeNode *tree_node);
static void l_close(BriTreeNode *tree_node);
static gboolean l_is_open(BriTreeNode *tree_node);
static gboolean l_can_open(BriTreeNode *tree_node);

static void bri_tree_node_class_init(BriTreeNodeClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	clazz->open = l_open;
	clazz->close = l_close;
	clazz->isOpen = l_is_open;
	clazz->canOpen = l_can_open;
	clazz->enlistChildren = NULL;
}

static void bri_tree_node_init(BriTreeNode *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	BriTreeNode *instance = BRI_TREE_NODE(object);
//	BriTreeNodePrivate *priv = bri_tree_node_get_instance_private(instance);
	G_OBJECT_CLASS(bri_tree_node_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(bri_tree_node_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}



void bri_tree_node_construct(BriTreeNode *node, BriTreeNode *parent_node) {
	BriTreeNodePrivate *priv = bri_tree_node_get_instance_private(node);
	priv->parent_node = parent_node;
	priv->children = NULL;
	priv->openCount = 0;
	priv->text = NULL;
}

BriTreeNode *bri_tree_node_new(BriTreeNode *parent_node) {
	BriTreeNode *result = g_object_new(BRI_TYPE_TREE_NODE, NULL);
	cat_ref_anounce(result);
	bri_tree_node_construct(result, parent_node);
	return result;
}

static void l_increase_version(BriTreeNode *tree_node) {
	BriTreeNodePrivate *priv = bri_tree_node_get_instance_private(tree_node);
	priv->versions.selfVersion++;
	BriTreeNode *sub_version = priv->parent_node;
	while(sub_version) {
		priv = bri_tree_node_get_instance_private(sub_version);
		priv->versions.subVersion++;
		sub_version = priv->parent_node;
	}
}

void bri_tree_node_open(BriTreeNode *tree_node) {
	BRI_TREE_NODE_GET_CLASS(tree_node)->open(tree_node);
}

void bri_tree_node_close(BriTreeNode *tree_node) {
	BRI_TREE_NODE_GET_CLASS(tree_node)->close(tree_node);
}

gboolean bri_tree_node_is_open(BriTreeNode *tree_node) {
	return BRI_TREE_NODE_GET_CLASS(tree_node)->isOpen(tree_node);
}

gboolean bri_tree_node_can_open(BriTreeNode *tree_node) {
	return BRI_TREE_NODE_GET_CLASS(tree_node)->canOpen(tree_node);
}


CatArrayWo *bri_tree_node_get_children(BriTreeNode *tree_node) {
	BriTreeNodePrivate *priv = bri_tree_node_get_instance_private(tree_node);
	return priv->children;
}

CatStringWo *bri_tree_node_get_text(BriTreeNode *tree_node) {
	BriTreeNodePrivate *priv = bri_tree_node_get_instance_private(tree_node);
	return priv->text;
}

void bri_tree_node_set_text(BriTreeNode *tree_node, CatStringWo *text) {
	BriTreeNodePrivate *priv = bri_tree_node_get_instance_private(tree_node);
	cat_ref_swap(priv->text, text);
	l_increase_version(tree_node);
}


void bri_tree_node_increase_sub_version(BriTreeNode *tree_node) {
	BriTreeNodePrivate *priv = bri_tree_node_get_instance_private(tree_node);
	priv->versions.subVersion++;
}

BriTreeNodeVersions bri_tree_node_get_versions(BriTreeNode *tree_node) {
	BriTreeNodePrivate *priv = bri_tree_node_get_instance_private(tree_node);
	return priv->versions;
}

static void l_open(BriTreeNode *tree_node) {
	BriTreeNodePrivate *priv = bri_tree_node_get_instance_private(tree_node);
	priv->openCount++;
	if (priv->openCount==1) {
		cat_log_debug("priv->openCount=%d", priv->openCount);
		BriTreeNodeClass *clazz = BRI_TREE_NODE_GET_CLASS(tree_node);
		if (clazz->enlistChildren!=NULL) {
			cat_unref_ptr(priv->children);
			CatArrayWo *children = clazz->enlistChildren(tree_node);
			priv->children = children;
			l_increase_version(tree_node);
		}
	}
}


static void l_close(BriTreeNode *tree_node) {
	BriTreeNodePrivate *priv = bri_tree_node_get_instance_private(tree_node);
	priv->openCount--;
	if (priv->openCount<=0) {
		priv->openCount = 0;
		cat_unref_ptr(priv->children);
		l_increase_version(tree_node);
	}
}

static gboolean l_is_open(BriTreeNode *tree_node) {
	BriTreeNodePrivate *priv = bri_tree_node_get_instance_private(tree_node);
	return priv->openCount>0;
}

static gboolean l_can_open(BriTreeNode *tree_node) {
	BriTreeNodePrivate *priv = bri_tree_node_get_instance_private(tree_node);
	return FALSE;
}
