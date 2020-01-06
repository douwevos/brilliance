/*
   File:    bridraeditor.h
   Project: brilliance-lib-jni
   Author:  Douwe Vos
   Date:    Dec 5, 2019
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

#ifndef BRIDRAEDITOR_H_
#define BRIDRAEDITOR_H_

#include <jni.h>
#include <dragonfly.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define BRI_TYPE_DRA_EDITOR              (bri_dra_editor_get_type())
#define BRI_DRA_EDITOR(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), bri_dra_editor_get_type(), BriDraEditor))
#define BRI_DRA_EDITOR_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), BRI_TYPE_DRA_EDITOR, BriDraEditorClass))
#define BRI_IS_DRA_EDITOR(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), BRI_TYPE_DRA_EDITOR))
#define BRI_IS_DRA_EDITOR_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), BRI_TYPE_DRA_EDITOR))
#define BRI_DRA_EDITOR_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), BRI_TYPE_DRA_EDITOR, BriDraEditorClass))

struct _BriDraEditor {
	DraEditor parent;
};

struct _BriDraEditorClass {
	DraEditorClass parent_class;
};

typedef struct _BriDraEditor               BriDraEditor;
typedef struct _BriDraEditorPrivate        BriDraEditorPrivate;
typedef struct _BriDraEditorClass          BriDraEditorClass;


GType bri_dra_editor_get_type();

BriDraEditor *bri_dra_editor_new();

void bri_dra_editor_construct(BriDraEditor *editor, JNIEnv *env, jobject editor_obj, ChaDocument *document, DraConnectorMap *connector_map, DraIConnectorRequestFactory *connector_factory, WorService *wor_service);

void bri_dra_editor_link(BriDraEditor *editor, JNIEnv *env, jobject java_link);

jobject bri_dra_editor_get_link(BriDraEditor *editor, JNIEnv *env);

G_END_DECLS

#endif /* BRIDRAEDITOR_H_ */
