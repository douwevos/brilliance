/*
   File:    bridradocumentview.c
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


#include "bridradocumentview.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "BriDraDocumentView"
#include <logging/catlog.h>

struct _BriDraDocumentViewPrivate {
	JavaVM *jvm;
	jweak weak_link;
};

G_DEFINE_TYPE_WITH_CODE(BriDraDocumentView, bri_dra_document_view, DRA_TYPE_DOCUMENT_VIEW,  // @suppress("Unused static function")
	G_ADD_PRIVATE(BriDraDocumentView)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void bri_dra_document_view_class_init(BriDraDocumentViewClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void bri_dra_document_view_init(BriDraDocumentView *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	BriDraDocumentView *instance = BRI_DRA_DOCUMENT_VIEW(object);
//	BriDraDocumentViewPrivate *priv = bri_dra_document_view_get_instance_private(instance);
	G_OBJECT_CLASS(bri_dra_document_view_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(bri_dra_document_view_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


BriDraDocumentView *bri_dra_document_view_new() {
	BriDraDocumentView *result = g_object_new(BRI_TYPE_DRA_DOCUMENT_VIEW, NULL);
	cat_ref_anounce(result);
	return result;
}

void bri_dra_document_view_construct(BriDraDocumentView *document_view, JNIEnv *env, jobject document_view_obj, ChaDocument *document, PangoContext *pango_context, GtkWidget *widget, CatStringWo *line_info_key) {
	BriDraDocumentViewPrivate *priv = bri_dra_document_view_get_instance_private(document_view);
	cat_log_debug("linking");
	priv->weak_link = (*env)->NewWeakGlobalRef(env, document_view_obj);
	(*env)->GetJavaVM(env, &(priv->jvm));
	dra_document_view_construct((DraDocumentView *) document_view, document, pango_context, widget, line_info_key);

}

jobject bri_dra_document_view_get_link(BriDraDocumentView *document_view, JNIEnv *env) {
	BriDraDocumentViewPrivate *priv = bri_dra_document_view_get_instance_private(document_view);
	return (*env)->NewLocalRef(env, priv->weak_link);
}

