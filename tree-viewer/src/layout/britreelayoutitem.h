/*
   File:    britreelayoutitem.h
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

#ifndef BRITREELAYOUTITEM_H_
#define BRITREELAYOUTITEM_H_

#include "../model/britreenoderef.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define BRI_TYPE_TREE_LAYOUT_ITEM              (bri_tree_layout_item_get_type())
#define BRI_TREE_LAYOUT_ITEM(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), bri_tree_layout_item_get_type(), BriTreeLayoutItem))
#define BRI_TREE_LAYOUT_ITEM_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), BRI_TYPE_TREE_LAYOUT_ITEM, BriTreeLayoutItemClass))
#define BRI_IS_TREE_LAYOUT_ITEM(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), BRI_TYPE_TREE_LAYOUT_ITEM))
#define BRI_IS_TREE_LAYOUT_ITEM_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), BRI_TYPE_TREE_LAYOUT_ITEM))
#define BRI_TREE_LAYOUT_ITEM_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), BRI_TYPE_TREE_LAYOUT_ITEM, BriTreeLayoutItemClass))

struct _BriTreeLayoutItem {
	GObject parent;
};

struct _BriTreeLayoutItemClass {
	GObjectClass parent_class;
};

typedef struct _BriTreeLayoutItem               BriTreeLayoutItem;
typedef struct _BriTreeLayoutItemPrivate        BriTreeLayoutItemPrivate;
typedef struct _BriTreeLayoutItemClass          BriTreeLayoutItemClass;


GType bri_tree_layout_item_get_type();

BriTreeLayoutItem *bri_tree_layout_item_new(BriTreeNodeRef *node);

BriTreeNodeRef *bri_tree_layout_item_get_node_ref(BriTreeLayoutItem *layout_item);

void bri_tree_layout_item_set_expanded(BriTreeLayoutItem *layout_item, gboolean expanded);

gboolean bri_tree_layout_item_is_expanded(BriTreeLayoutItem *layout_item);

void bri_tree_layout_item_set_children(BriTreeLayoutItem *layout_item, CatArrayWo *children);
CatArrayWo *bri_tree_layout_item_get_children(BriTreeLayoutItem *layout_item);

CatStringWo *bri_tree_layout_item_get_text(BriTreeLayoutItem *layout_item);

gboolean bri_tree_layout_item_can_expand(BriTreeLayoutItem *layout_item);

void bri_tree_layout_item_set_level(BriTreeLayoutItem *layout_item, int level);
int bri_tree_layout_item_get_level(BriTreeLayoutItem *layout_item);

G_END_DECLS

#endif /* BRITREELAYOUTITEM_H_ */
