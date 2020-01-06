/*
   File:    britreelayout.h
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

#ifndef BRITREELAYOUT_H_
#define BRITREELAYOUT_H_

#include "../model/britreemodel.h"
#include "britreelayoutitem.h"
#include <caterpillar.h>
#include <gtk/gtk.h>


G_BEGIN_DECLS

#define BRI_TYPE_TREE_LAYOUT              (bri_tree_layout_get_type())
#define BRI_TREE_LAYOUT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), bri_tree_layout_get_type(), BriTreeLayout))
#define BRI_TREE_LAYOUT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), BRI_TYPE_TREE_LAYOUT, BriTreeLayoutClass))
#define BRI_IS_TREE_LAYOUT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), BRI_TYPE_TREE_LAYOUT))
#define BRI_IS_TREE_LAYOUT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), BRI_TYPE_TREE_LAYOUT))
#define BRI_TREE_LAYOUT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), BRI_TYPE_TREE_LAYOUT, BriTreeLayoutClass))

struct _BriTreeLayout {
	GObject parent;
};

struct _BriTreeLayoutClass {
	GObjectClass parent_class;
};

typedef struct _BriTreeLayout               BriTreeLayout;
typedef struct _BriTreeLayoutPrivate        BriTreeLayoutPrivate;
typedef struct _BriTreeLayoutClass          BriTreeLayoutClass;


GType bri_tree_layout_get_type();

BriTreeLayout *bri_tree_layout_new(BriTreeModel *model);

void bri_tree_layout_validate(BriTreeLayout *layout, PangoContext *pango_context);

void bri_tree_layout_draw(BriTreeLayout *layout, PangoContext *pango_context,
		cairo_t *cairo);

BriTreeLayoutItem *bri_tree_layout_model_node_at(BriTreeLayout *layout, int xmouse, int ymouse);

gboolean bri_tree_layout_select(BriTreeLayout *layout, BriTreeLayoutItem *item, gboolean toggle, gboolean ctrlPressed, gboolean shiftPressed);

G_END_DECLS

#endif /* BRITREELAYOUT_H_ */
