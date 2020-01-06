/*
   File:    bridraeditorpanel.h
   Project: brilliance-lib-jni
   Author:  Douwe Vos
   Date:    Dec 10, 2019
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

#ifndef BRIDRAEDITORPANEL_H_
#define BRIDRAEDITORPANEL_H_

#include <jni.h>
#include <dragonfly.h>
#include <caterpillar.h>
#include <leafhopper.h>

G_BEGIN_DECLS

#define BRI_TYPE_DRA_EDITOR_PANEL              (bri_dra_editor_panel_get_type())
#define BRI_DRA_EDITOR_PANEL(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), bri_dra_editor_panel_get_type(), BriDraEditorPanel))
#define BRI_DRA_EDITOR_PANEL_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), BRI_TYPE_DRA_EDITOR_PANEL, BriDraEditorPanelClass))
#define BRI_IS_DRA_EDITOR_PANEL(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), BRI_TYPE_DRA_EDITOR_PANEL))
#define BRI_IS_DRA_EDITOR_PANEL_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), BRI_TYPE_DRA_EDITOR_PANEL))
#define BRI_DRA_EDITOR_PANEL_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), BRI_TYPE_DRA_EDITOR_PANEL, BriDraEditorPanelClass))

struct _BriDraEditorPanel {
	DraEditorPanel parent;
};

struct _BriDraEditorPanelClass {
	DraEditorPanelClass parent_class;
};

typedef struct _BriDraEditorPanel               BriDraEditorPanel;
typedef struct _BriDraEditorPanelPrivate        BriDraEditorPanelPrivate;
typedef struct _BriDraEditorPanelClass          BriDraEditorPanelClass;


GType bri_dra_editor_panel_get_type();

BriDraEditorPanel *bri_dra_editor_panel_new();

void bri_dra_editor_panel_construct(BriDraEditorPanel *editor_panel, JNIEnv *env, jobject editor_panel_obj, LeaIPanelOwner *panel_owner, ChaDocument *document, DraIConnectorRequestFactory *request_factory);

void bri_dra_editor_panel_link(BriDraEditorPanel *editor_panel, JNIEnv *env, jobject java_link);

G_END_DECLS

#endif /* BRIDRAEDITORPANEL_H_ */
