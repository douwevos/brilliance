/*
   File:    bridraeditor.c
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

#include "bridraeditor.h"
#include "../brilliance_native_helper.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "BriDraEditor"
#include <logging/catlog.h>

struct _BriDraEditorPrivate {
	JavaVM *jvm;
	jweak weak_link;
};

G_DEFINE_TYPE_WITH_CODE(BriDraEditor, bri_dra_editor, DRA_TYPE_EDITOR,  // @suppress("Unused static function")
	G_ADD_PRIVATE(BriDraEditor)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static ChaDocumentView *l_initialize_document_view(ChaEditor *editor, ChaDocument *document, PangoContext *pango_context, GtkWidget *widget);
static ChaUow *l_create_insert_or_replace_uow(ChaEditor *editor, ChaEditMode mode, CatStringWo *text);


static void bri_dra_editor_class_init(BriDraEditorClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	ChaEditorClass *cha_class = CHA_EDITOR_CLASS(clazz);
	cha_class->initializeDocumentView = l_initialize_document_view;
	cha_class->createInsertOrReplaceUow = l_create_insert_or_replace_uow;
}

static void bri_dra_editor_init(BriDraEditor *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	BriDraEditor *instance = BRI_DRA_EDITOR(object);
	BriDraEditorPrivate *priv = bri_dra_editor_get_instance_private(instance);
	G_OBJECT_CLASS(bri_dra_editor_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(bri_dra_editor_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


BriDraEditor *bri_dra_editor_new() {
	BriDraEditor *result = g_object_new(BRI_TYPE_DRA_EDITOR, NULL);
	cat_ref_anounce(result);
	return result;
}

void bri_dra_editor_construct(BriDraEditor *editor, JNIEnv *env, jobject editor_obj, ChaDocument *document, DraConnectorMap *connector_map, DraIConnectorRequestFactory *connector_factory, WorService *wor_service) {
	cat_log_debug("construct:%p", editor);
	BriDraEditorPrivate *priv = bri_dra_editor_get_instance_private(editor);
	(*env)->GetJavaVM(env, &(priv->jvm));
	cat_log_debug("construct:%p, vm=%p, priv=%p", editor, priv->jvm, priv);
	priv->weak_link = (*env)->NewWeakGlobalRef(env, editor_obj);
	dra_editor_construct((DraEditor *) editor, document, connector_map, connector_factory, wor_service);
}


void bri_dra_editor_link(BriDraEditor *editor, JNIEnv *env, jobject java_link) {
	BriDraEditorPrivate *priv = bri_dra_editor_get_instance_private(editor);
	priv->weak_link = (*env)->NewWeakGlobalRef(env, java_link);
}

jobject bri_dra_editor_get_link(BriDraEditor *editor, JNIEnv *env) {
	BriDraEditorPrivate *priv = bri_dra_editor_get_instance_private(editor);
	return (*env)->NewLocalRef(env, priv->weak_link);
}

//ChaDocumentView *(*initializeDocumentView)(ChaEditor *editor, ChaDocument *document, PangoContext *pango_context, GtkWidget *widget);

static ChaDocumentView *l_initialize_document_view(ChaEditor *editor, ChaDocument *document, PangoContext *pango_context, GtkWidget *widget) {
	BriDraEditorPrivate *priv = bri_dra_editor_get_instance_private((BriDraEditor *) editor);
	cat_log_debug("start:%p", editor);
	cat_log_debug("start:%p, vm=%p, priv=%p", editor, priv->jvm, priv);

	JNIEnv* env = brilliance_attach_java(priv->jvm);
	cat_log_debug("attached");
	jobject local_link = (*env)->NewLocalRef(env, priv->weak_link);
	cat_log_debug("local_link=%p", local_link);
	jclass obj_class = (*env)->GetObjectClass(env, local_link);
	cat_log_debug("obj_class=%p", obj_class);
	if ((*env)->ExceptionCheck(env)) {
		cat_log_error("in GetObjectClass");
		brilliance_detach_java(priv->jvm);
		return NULL;
	}

	jobject jdocument = NULL;
	if (document) {
		jclass document_cls = (*env)->FindClass(env, "net/natpad/brilliance/chameleon/Document");
		jmethodID mi_document_constructor = (*env)->GetMethodID(env, document_cls, "<init>", "(J)V");
		jdocument = (*env)->NewObject(env, document_cls, mi_document_constructor, (jlong) document);
		g_object_ref(document);
	}
	cat_log_debug("jdocument=%p", jdocument);

	jlong jpango_context = (jlong) pango_context;

	jobject jgtk_widget = NULL;
	if (widget) {
		jclass gtk_widget_cls = (*env)->FindClass(env, "net/natpad/brilliance/gtk/GtkWidget");
		jmethodID mi_gtk_widget_constructor = (*env)->GetMethodID(env, gtk_widget_cls, "<init>", "(J)V");
		jgtk_widget = (*env)->NewObject(env, gtk_widget_cls, mi_gtk_widget_constructor, (jlong) widget);
		g_object_ref(widget);
	}
	cat_log_debug("jgtk_widget=%p", jgtk_widget);

	jmethodID mi_init_document_view = (*env)->GetMethodID(env, obj_class, "initializeDocumentView", "(Lnet/natpad/brilliance/chameleon/Document;JLnet/natpad/brilliance/gtk/GtkWidget;)Lnet/natpad/brilliance/chameleon/DocumentView;");
	cat_log_debug("mi_init_document_view=%p", mi_init_document_view);

	jobject o = (*env)->CallObjectMethod(env, local_link, mi_init_document_view, jdocument, jpango_context, jgtk_widget);
	cat_log_debug("mi_init_document_view=%p", mi_init_document_view);

	ChaDocumentView *result = brilliance_as_object(env, o);
	brilliance_detach_java(priv->jvm);
	return result;
}

static ChaUow *l_create_insert_or_replace_uow(ChaEditor *editor, ChaEditMode mode, CatStringWo *text) {
	BriDraEditorPrivate *priv = bri_dra_editor_get_instance_private((BriDraEditor *) editor);
	JNIEnv* env = brilliance_attach_java(priv->jvm);
	jobject local_link = (*env)->NewLocalRef(env, priv->weak_link);
	cat_log_debug("local_link=%p", local_link);
	jclass obj_class = (*env)->GetObjectClass(env, local_link);

	jstring jtext = (*env)->NewStringUTF(env, cat_string_wo_getchars(text));
	jmethodID mi_create_insert_or_replace_uow = (*env)->GetMethodID(env, obj_class, "createInsertOrReplaceUow", "(ILjava/lang/String;)Lnet/natpad/brilliance/chameleon/uow/Uow;");
	cat_log_debug("mi_create_insert_or_replace_uow=%p", mi_create_insert_or_replace_uow);

	jobject out = (*env)->CallObjectMethod(env, local_link, mi_create_insert_or_replace_uow, mode, jtext);

	ChaUow *result = brilliance_as_object(env, out);
	cat_ref_ptr(result);
	brilliance_detach_java(priv->jvm);
	return result;

}
