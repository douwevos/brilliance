/*
   File:    brichadocumentview.h
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

#ifndef BRICHADOCUMENTVIEW_H_
#define BRICHADOCUMENTVIEW_H_

#include <jni.h>
#include <caterpillar.h>
#include <chameleon.h>

G_BEGIN_DECLS

#define BRI_TYPE_CHA_DOCUMENT_VIEW              (bri_cha_document_view_get_type())
#define BRI_CHA_DOCUMENT_VIEW(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), bri_cha_document_view_get_type(), BriChaDocumentView))
#define BRI_CHA_DOCUMENT_VIEW_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), BRI_TYPE_CHA_DOCUMENT_VIEW, BriChaDocumentViewClass))
#define BRI_IS_CHA_DOCUMENT_VIEW(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), BRI_TYPE_CHA_DOCUMENT_VIEW))
#define BRI_IS_CHA_DOCUMENT_VIEW_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), BRI_TYPE_CHA_DOCUMENT_VIEW))
#define BRI_CHA_DOCUMENT_VIEW_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), BRI_TYPE_CHA_DOCUMENT_VIEW, BriChaDocumentViewClass))

struct _BriChaDocumentView {
	ChaDocumentView parent;
};

struct _BriChaDocumentViewClass {
	ChaDocumentViewClass parent_class;
};

typedef struct _BriChaDocumentView               BriChaDocumentView;
typedef struct _BriChaDocumentViewPrivate        BriChaDocumentViewPrivate;
typedef struct _BriChaDocumentViewClass          BriChaDocumentViewClass;


GType bri_cha_document_view_get_type();

BriChaDocumentView *bri_cha_document_view_new();

void bri_cha_document_view_construct(BriChaDocumentView *document_view,JNIEnv *env, jobject document_view_obj, ChaDocument *document, PangoContext *pango_context, GtkWidget *widget);

jobject bri_cha_document_view_get_link(BriChaDocumentView *document_view,JNIEnv *env);

G_END_DECLS

#endif /* BRICHADOCUMENTVIEW_H_ */
