/*
   File:    britreemodel.h
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

#ifndef BRITREEMODEL_H_
#define BRITREEMODEL_H_

#include "britreenoderef.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define BRI_TYPE_TREE_MODEL              (bri_tree_model_get_type())
#define BRI_TREE_MODEL(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), bri_tree_model_get_type(), BriTreeModel))
#define BRI_TREE_MODEL_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), BRI_TYPE_TREE_MODEL, BriTreeModelClass))
#define BRI_IS_TREE_MODEL(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), BRI_TYPE_TREE_MODEL))
#define BRI_IS_TREE_MODEL_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), BRI_TYPE_TREE_MODEL))
#define BRI_TREE_MODEL_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), BRI_TYPE_TREE_MODEL, BriTreeModelClass))

struct _BriTreeModel {
	GObject parent;
};

struct _BriTreeModelClass {
	GObjectClass parent_class;
};

typedef struct _BriTreeModel               BriTreeModel;
typedef struct _BriTreeModelPrivate        BriTreeModelPrivate;
typedef struct _BriTreeModelClass          BriTreeModelClass;


GType bri_tree_model_get_type();

BriTreeModel *bri_tree_model_new();

BriTreeNodeRef *bri_tree_model_get_root_ref(BriTreeModel *model);

G_END_DECLS


#endif /* BRITREEMODEL_H_ */
