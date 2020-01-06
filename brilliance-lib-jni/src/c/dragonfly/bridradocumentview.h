/*
   File:    bridradocumentview.h
   Project: brilliance-lib-jni
   Author:  Douwe Vos
   Date:    Dec 19, 2019
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

#ifndef BRIDRADOCUMENTVIEW_H_
#define BRIDRADOCUMENTVIEW_H_

#include <jni.h>
#include <caterpillar.h>
#include <dragonfly.h>

G_BEGIN_DECLS

#define BRI_TYPE_DRA_DOCUMENT_VIEW              (bri_dra_document_view_get_type())
#define BRI_DRA_DOCUMENT_VIEW(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), bri_dra_document_view_get_type(), BriDraDocumentView))
#define BRI_DRA_DOCUMENT_VIEW_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), BRI_TYPE_DRA_DOCUMENT_VIEW, BriDraDocumentViewClass))
#define BRI_IS_DRA_DOCUMENT_VIEW(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), BRI_TYPE_DRA_DOCUMENT_VIEW))
#define BRI_IS_DRA_DOCUMENT_VIEW_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), BRI_TYPE_DRA_DOCUMENT_VIEW))
#define BRI_DRA_DOCUMENT_VIEW_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), BRI_TYPE_DRA_DOCUMENT_VIEW, BriDraDocumentViewClass))

struct _BriDraDocumentView {
	DraDocumentView parent;
};

struct _BriDraDocumentViewClass {
	DraDocumentViewClass parent_class;
};

typedef struct _BriDraDocumentView               BriDraDocumentView;
typedef struct _BriDraDocumentViewPrivate        BriDraDocumentViewPrivate;
typedef struct _BriDraDocumentViewClass          BriDraDocumentViewClass;


GType bri_dra_document_view_get_type();

BriDraDocumentView *bri_dra_document_view_new();

void bri_dra_document_view_construct(BriDraDocumentView *document_view, JNIEnv *env, jobject document_view_obj, ChaDocument *document, PangoContext *pango_context, GtkWidget *widget, CatStringWo *line_info_key);

jobject bri_dra_document_view_get_link(BriDraDocumentView *document_view, JNIEnv *env);

G_END_DECLS

#endif /* BRIDRADOCUMENTVIEW_H_ */
