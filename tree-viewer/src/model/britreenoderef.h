/*
   File:    britreenoderef.h
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

#ifndef BRITREENODEREF_H_
#define BRITREENODEREF_H_

#include "britreenode.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define BRI_TYPE_TREE_NODE_REF              (bri_tree_node_ref_get_type())
#define BRI_TREE_NODE_REF(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), bri_tree_node_ref_get_type(), BriTreeNodeRef))
#define BRI_TREE_NODE_REF_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), BRI_TYPE_TREE_NODE_REF, BriTreeNodeRefClass))
#define BRI_IS_TREE_NODE_REF(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), BRI_TYPE_TREE_NODE_REF))
#define BRI_IS_TREE_NODE_REF_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), BRI_TYPE_TREE_NODE_REF))
#define BRI_TREE_NODE_REF_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), BRI_TYPE_TREE_NODE_REF, BriTreeNodeRefClass))

struct _BriTreeModel;

struct _BriTreeNodeRef {
	GObject parent;
};

struct _BriTreeNodeRefClass {
	GObjectClass parent_class;
};

typedef struct _BriTreeNodeRef               BriTreeNodeRef;
typedef struct _BriTreeNodeRefPrivate        BriTreeNodeRefPrivate;
typedef struct _BriTreeNodeRefClass          BriTreeNodeRefClass;


GType bri_tree_node_ref_get_type();

BriTreeNodeRef *bri_tree_node_ref_new(struct _BriTreeModel *tree_model, BriTreeNode *tree_node, BriTreeNodeRef *parent_ref);

BriTreeNodeVersions bri_tree_node_ref_get_versions(BriTreeNodeRef *node);

CatArrayWo *bri_tree_node_ref_enlist_children(BriTreeNodeRef *node);

CatStringWo *bri_tree_node_ref_get_text(BriTreeNodeRef *node);

void bri_tree_node_ref_set_text(BriTreeNodeRef *node, CatStringWo *text);

void bri_tree_node_ref_set_opened(BriTreeNodeRef *node, gboolean opened);

gboolean bri_tree_node_ref_is_opened(BriTreeNodeRef *node);
gboolean bri_tree_node_ref_can_open(BriTreeNodeRef *node);

G_END_DECLS

#endif /* BRITREENODEREF_H_ */
