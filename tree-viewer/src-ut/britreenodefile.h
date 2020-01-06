/*
   File:    britreenodefile.h
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

#ifndef BRITREENODEFILE_H_
#define BRITREENODEFILE_H_

#include "../src/model/britreenode.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define BRI_TYPE_TREE_NODE_FILE              (bri_tree_node_file_get_type())
#define BRI_TREE_NODE_FILE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), bri_tree_node_file_get_type(), BriTreeNodeFile))
#define BRI_TREE_NODE_FILE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), BRI_TYPE_TREE_NODE_FILE, BriTreeNodeFileClass))
#define BRI_IS_TREE_NODE_FILE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), BRI_TYPE_TREE_NODE_FILE))
#define BRI_IS_TREE_NODE_FILE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), BRI_TYPE_TREE_NODE_FILE))
#define BRI_TREE_NODE_FILE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), BRI_TYPE_TREE_NODE_FILE, BriTreeNodeFileClass))

struct _BriTreeNodeFile {
	BriTreeNode parent;
};

struct _BriTreeNodeFileClass {
	BriTreeNodeClass parent_class;
};

typedef struct _BriTreeNodeFile               BriTreeNodeFile;
typedef struct _BriTreeNodeFilePrivate        BriTreeNodeFilePrivate;
typedef struct _BriTreeNodeFileClass          BriTreeNodeFileClass;


GType bri_tree_node_file_get_type();

BriTreeNodeFile *bri_tree_node_file_new();

G_END_DECLS

#endif /* BRITREENODEFILE_H_ */
