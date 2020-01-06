/*
   File:    britreemodel.c
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

#include "britreemodel.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_ALL
#define CAT_LOG_CLAZZ "BriTreeModel"
#include <logging/catlog.h>

struct _BriTreeModelPrivate {
	BriTreeNode *root;
};

G_DEFINE_TYPE_WITH_CODE(BriTreeModel, bri_tree_model, G_TYPE_OBJECT,  // @suppress("Unused static function")
	G_ADD_PRIVATE(BriTreeModel)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void bri_tree_model_class_init(BriTreeModelClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void bri_tree_model_init(BriTreeModel *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	BriTreeModel *instance = BRI_TREE_MODEL(object);
//	BriTreeModelPrivate *priv = bri_tree_model_get_instance_private(instance);
	G_OBJECT_CLASS(bri_tree_model_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(bri_tree_model_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


BriTreeModel *bri_tree_model_new(BriTreeNode *root) {
	BriTreeModel *result = g_object_new(BRI_TYPE_TREE_MODEL, NULL);
	cat_ref_anounce(result);
	BriTreeModelPrivate *priv = bri_tree_model_get_instance_private(result);
	priv->root = cat_ref_ptr(root);
	return result;
}


BriTreeNodeRef *bri_tree_model_get_root_ref(BriTreeModel *model) {
	BriTreeModelPrivate *priv = bri_tree_model_get_instance_private(model);
	return bri_tree_node_ref_new(model, priv->root, NULL);

}
