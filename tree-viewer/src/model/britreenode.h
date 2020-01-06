/*
   File:    britreenode.h
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

#ifndef BRITREENODE_H_
#define BRITREENODE_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define BRI_TYPE_TREE_NODE              (bri_tree_node_get_type())
#define BRI_TREE_NODE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), bri_tree_node_get_type(), BriTreeNode))
#define BRI_TREE_NODE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), BRI_TYPE_TREE_NODE, BriTreeNodeClass))
#define BRI_IS_TREE_NODE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), BRI_TYPE_TREE_NODE))
#define BRI_IS_TREE_NODE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), BRI_TYPE_TREE_NODE))
#define BRI_TREE_NODE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), BRI_TYPE_TREE_NODE, BriTreeNodeClass))

struct _BriTreeNodeVersions {
	/* increases every time the children array is changed, or the node info itself changed */
	int selfVersion;

	/* increases every time one of the children its selfVersion changed */
	int subVersion;
};

typedef struct _BriTreeNodeVersions BriTreeNodeVersions;

struct _BriTreeNode {
	GObject parent;
};

typedef struct _BriTreeNode               BriTreeNode;

struct _BriTreeNodeClass {
	GObjectClass parent_class;
	void (*open)(BriTreeNode *tree_node);
	void (*close)(BriTreeNode *tree_node);
	gboolean (*isOpen)(BriTreeNode *tree_node);
	gboolean (*canOpen)(BriTreeNode *tree_node);
	CatArrayWo *(*enlistChildren)(BriTreeNode *tree_node);
};

typedef struct _BriTreeNodePrivate        BriTreeNodePrivate;
typedef struct _BriTreeNodeClass          BriTreeNodeClass;


GType bri_tree_node_get_type();

void bri_tree_node_construct(BriTreeNode *node, BriTreeNode *parent_node);

BriTreeNode *bri_tree_node_new();

void bri_tree_node_open(BriTreeNode *tree_node);
void bri_tree_node_close(BriTreeNode *tree_node);

CatArrayWo *bri_tree_node_get_children(BriTreeNode *tree_node);

gboolean bri_tree_node_is_open(BriTreeNode *tree_node);
gboolean bri_tree_node_can_open(BriTreeNode *tree_node);

CatStringWo *bri_tree_node_get_text(BriTreeNode *tree_node);
void bri_tree_node_set_text(BriTreeNode *tree_node, CatStringWo *text);

void bri_tree_node_increase_sub_version(BriTreeNode *tree_node);
BriTreeNodeVersions bri_tree_node_get_versions(BriTreeNode *tree_node);

G_END_DECLS


#endif /* BRITREENODE_H_ */
