/*
   File:    britreenodefile.c
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


#include "britreenodefile.h"
#include <gio/gio.h>


#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_ALL
#define CAT_LOG_CLAZZ "BriTreeNodeFile"
#include <logging/catlog.h>

struct _BriTreeNodeFilePrivate {
	GFile *file;
};

static void l_init_istringable(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(BriTreeNodeFile, bri_tree_node_file, BRI_TYPE_TREE_NODE,  // @suppress("Unused static function")
	G_ADD_PRIVATE(BriTreeNodeFile)
	G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_init_istringable)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static gboolean l_can_open(BriTreeNode *tree_node);
static CatArrayWo *l_enlist_children(BriTreeNode *tree_node);


static void bri_tree_node_file_class_init(BriTreeNodeFileClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	BriTreeNodeClass *node_class = BRI_TREE_NODE_CLASS(clazz);
	node_class->canOpen = l_can_open;
	node_class->enlistChildren = l_enlist_children;
}

static void bri_tree_node_file_init(BriTreeNodeFile *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	BriTreeNodeFile *instance = BRI_TREE_NODE_FILE(object);
//	BriTreeNodeFilePrivate *priv = bri_tree_node_file_get_instance_private(instance);
	G_OBJECT_CLASS(bri_tree_node_file_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(bri_tree_node_file_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


BriTreeNodeFile *bri_tree_node_file_new() {
	BriTreeNodeFile *result = g_object_new(BRI_TYPE_TREE_NODE_FILE, NULL);
	cat_ref_anounce(result);
	BriTreeNodeFilePrivate *priv = bri_tree_node_file_get_instance_private(result);
	bri_tree_node_construct((BriTreeNode *) result, NULL);
	priv->file = g_file_new_for_path("/");
	return result;
}


BriTreeNodeFile *bri_tree_node_file_new_child(BriTreeNodeFile *parent_node, GFile *file) {
	BriTreeNodeFile *result = g_object_new(BRI_TYPE_TREE_NODE_FILE, NULL);
	cat_ref_anounce(result);
	BriTreeNodeFilePrivate *priv = bri_tree_node_file_get_instance_private(result);
	bri_tree_node_construct((BriTreeNode *) result, parent_node);
	priv->file = cat_ref_ptr(file);
	GFileInfo *info = g_file_query_info(priv->file, G_FILE_ATTRIBUTE_STANDARD_DISPLAY_NAME, G_FILE_QUERY_INFO_NONE, NULL, NULL);
	const char *name = g_file_info_get_attribute_string(info, G_FILE_ATTRIBUTE_STANDARD_DISPLAY_NAME);
	CatStringWo *text = cat_string_wo_new_with(name);
	bri_tree_node_set_text(result, text);
	cat_unref_ptr(info);
	return result;
}


static gboolean l_can_open(BriTreeNode *tree_node) {
	BriTreeNodeFilePrivate *priv = bri_tree_node_file_get_instance_private((BriTreeNodeFile *) tree_node);
	GFileInfo *info = g_file_query_info(priv->file, G_FILE_ATTRIBUTE_STANDARD_TYPE, G_FILE_QUERY_INFO_NONE, NULL, NULL);
	gboolean result = FALSE;
	if (info) {
		GFileType tp = (GFileType) g_file_info_get_attribute_uint32(info, G_FILE_ATTRIBUTE_STANDARD_TYPE);
		if (tp == G_FILE_TYPE_DIRECTORY) {
			result = TRUE;
		}
		cat_unref_ptr(info);
	}
	return result;
}


static CatArrayWo *l_enlist_children(BriTreeNode *tree_node) {
	cat_log_debug("l_enlist_children");
	BriTreeNodeFilePrivate *priv = bri_tree_node_file_get_instance_private((BriTreeNodeFile *) tree_node);
	CatArrayWo *result = cat_array_wo_new();
	GFileEnumerator *enumerator = g_file_enumerate_children(priv->file, "*", G_FILE_QUERY_INFO_NOFOLLOW_SYMLINKS, NULL, NULL);
	while(TRUE) {
		GFileInfo *info;
		if (!g_file_enumerator_iterate(enumerator, &info, NULL, NULL, NULL)) {
			break;
		}
		if (!info)
			break;
		const char *name = g_file_info_get_name(info);
		cat_log_debug("name=%s", name);
		GFile *f = g_file_get_child(priv->file, name);
		BriTreeNodeFile *child_node = bri_tree_node_file_new_child(tree_node, f);
		cat_array_wo_append(result, (GObject *) child_node);
		cat_unref_ptr(child_node);
	}
	return result;
}


static void l_print(CatIStringable *self, struct _CatStringWo *append_to) {
	BriTreeNodeFile *instance = BRI_TREE_NODE_FILE(self);
	BriTreeNodeFilePrivate *priv = bri_tree_node_file_get_instance_private(instance);

	CatStringWo *text = bri_tree_node_get_text(instance);
	cat_string_wo_format(append_to, "BriTreeNodeFile[%O]", text);
}


static void l_init_istringable(CatIStringableInterface *iface) {
	iface->print = l_print;
}
