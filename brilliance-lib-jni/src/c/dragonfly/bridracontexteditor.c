/*
   File:    bridracontexteditor.c
   Project: brilliance-lib-jni
   Author:  Douwe Vos
   Date:    Nov 17, 2019
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

#include "bridracontexteditor.h"
#include "../brilliance_native_helper.h"
#include "../chameleon/brichacursorwo.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "BriDraContextEditor"
#include <logging/catlog.h>

struct _BriDraContextEditorPrivate {
	JavaVM *jvm;
	jobject java_link;
};


static void l_ac_content_provider_iface_init(DraIAcContentProviderInterface *iface);

G_DEFINE_TYPE_WITH_CODE(BriDraContextEditor, bri_dra_context_editor, DRA_TYPE_CONTEXT_EDITOR,  // @suppress("Unused static function")
	G_ADD_PRIVATE(BriDraContextEditor)
	G_IMPLEMENT_INTERFACE(DRA_TYPE_IAC_CONTENT_PROVIDER, l_ac_content_provider_iface_init)

);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static DraControlLeftRight l_calculate_control_left_right(DraContextEditor *context_editor, ChaLineWo *line, ChaCursorWo *cursor);
static void l_open_declaration(DraContextEditor *context_editor, ChaCursorWo *cursor);
static void l_format(DraContextEditor *context_editor);
static void l_context_insert_text(DraContextEditor *context_editor, CatStringWo *text);

static void bri_dra_context_editor_class_init(BriDraContextEditorClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	DraContextEditorClass *dra_class = DRA_CONTEXT_EDITOR_CLASS(clazz);
	dra_class->calculateControlLeftRight = l_calculate_control_left_right;
	dra_class->openDeclaration = l_open_declaration;
	dra_class->format = l_format;
	dra_class->contextInsertText = l_context_insert_text;
}

static void bri_dra_context_editor_init(BriDraContextEditor *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	BriDraContextEditor *instance = BRI_DRA_CONTEXT_EDITOR(object);
//	BriDraContextEditorPrivate *priv = bri_dra_context_editor_get_instance_private(instance);
	G_OBJECT_CLASS(bri_dra_context_editor_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(bri_dra_context_editor_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


BriDraContextEditor *bri_dra_context_editor_new(JNIEnv *env, jobject link, DraEditorPanel *editor_panel) {
	BriDraContextEditor *result = g_object_new(BRI_DRA_TYPE_CONTEXT_EDITOR, NULL);
	cat_ref_anounce(result);
	BriDraContextEditorPrivate *priv = bri_dra_context_editor_get_instance_private(result);
	dra_context_editor_construct((DraContextEditor *) result, editor_panel);
	cat_log_error("gogogogo");
	(*env)->GetJavaVM(env, &(priv->jvm));
	priv->java_link = (*env)->NewGlobalRef(env, link);
	return result;
}

void bri_dra_context_editor_link(BriDraContextEditor *request, jobject java_link) {
	BriDraContextEditorPrivate *priv = bri_dra_context_editor_get_instance_private(request);
	priv->java_link = java_link;

}


static void l_enlist_content(DraIAcContentProvider *content_provider, DraAcContext *ac_context) {
	BriDraContextEditor *aug_request = BRI_DRA_CONTEXT_EDITOR(content_provider);
	BriDraContextEditorPrivate *priv = bri_dra_context_editor_get_instance_private(aug_request);
	cat_log_debug("enlist_content:0");
	JNIEnv *env = brilliance_attach_java(priv->jvm);
	cat_log_debug("enlist_content:1:priv->java_link=%p", priv->java_link);
	jclass linkClass = (*env)->GetObjectClass(env, priv->java_link);
	cat_log_debug("enlist_content:2");
	jmethodID mid = (*env)->GetMethodID(env, linkClass, "enlistContent", "(Lnet/natpad/brilliance/dragonfly/AcContext;)V");
	cat_log_debug("enlist_content:3");
	if (mid) {

		jclass ac_context_cls = (*env)->FindClass(env, "net/natpad/brilliance/dragonfly/AcContext");
		cat_log_debug("create_req:8:%p", ac_context_cls);
		jmethodID mod_r = (*env)->GetMethodID(env, ac_context_cls, "<init>", "(J)V");
		cat_log_debug("create_req:9:%p", mod_r);
		jobject jaccontext = (*env)->NewObject(env, ac_context_cls, mod_r, (jlong) ac_context);
		cat_log_debug("create_req:10:%p", jaccontext);
		g_object_ref(ac_context);
		(*env)->CallVoidMethod(env, priv->java_link, mid, jaccontext);

	}
	cat_log_debug("enlist_content:4");
	brilliance_detach_java(priv->jvm);
}

static void l_ac_content_provider_iface_init(DraIAcContentProviderInterface *iface) {
	iface->enlistContent = l_enlist_content;
}


static DraControlLeftRight l_calculate_control_left_right(DraContextEditor *dra_context_editor, ChaLineWo *line, ChaCursorWo *cursor) {
	BriDraContextEditor *context_editor = BRI_DRA_CONTEXT_EDITOR(dra_context_editor);
	BriDraContextEditorPrivate *priv = bri_dra_context_editor_get_instance_private(context_editor);

	DraControlLeftRight result;
	result.left = -1;
	result.right = -1;
	JNIEnv *env = brilliance_attach_java(priv->jvm);
	cat_log_debug("1:priv->java_link=%p", priv->java_link);
	jclass linkClass = (*env)->GetObjectClass(env, priv->java_link);
	cat_log_debug("2");
	jmethodID mid = (*env)->GetMethodID(env, linkClass, "calculateControlLeftRight", "(Lnet/natpad/brilliance/chameleon/Cursor;)Lnet/natpad/brilliance/dragonfly/ControlLeftRight;");
	cat_log_debug("3");
	if (mid) {
		if (cursor) {
			DraEditorPanel *editor_panel = dra_context_editor_get_panel(dra_context_editor);
			DraEditor *editor = dra_editor_panel_get_editor(editor_panel);
			ChaDocument *document = cha_editor_get_document((ChaEditor *) editor);
			ChaRevisionWo *a_revision_ref = cha_document_get_current_revision_ref(document);
			cat_unref_ptr(a_revision_ref);
		}

		jobject jcursor = bri_cha_cursor_wo_as_java(env, cursor);
		jobject ctr_left_right_obj = (*env)->CallObjectMethod(env, priv->java_link, mid, jcursor);
		if (ctr_left_right_obj != NULL) {
			jclass ctr_lr_class = (*env)->GetObjectClass(env, ctr_left_right_obj);
		    jfieldID fid_left = (*env)->GetFieldID(env, ctr_lr_class, "left", "I");
			result.left = (*env)->GetIntField(env, ctr_left_right_obj, fid_left);
		    jfieldID fid_right = (*env)->GetFieldID(env, ctr_lr_class, "right", "I");
			result.right = (*env)->GetIntField(env, ctr_left_right_obj, fid_right);
		}
	}
	brilliance_detach_java(priv->jvm);
	return result;
}

static void l_open_declaration(DraContextEditor *dra_context_editor, ChaCursorWo *cursor) {
	BriDraContextEditor *context_editor = BRI_DRA_CONTEXT_EDITOR(dra_context_editor);
	BriDraContextEditorPrivate *priv = bri_dra_context_editor_get_instance_private(context_editor);
	JNIEnv *env = brilliance_attach_java(priv->jvm);
	jclass linkClass = (*env)->GetObjectClass(env, priv->java_link);
	cat_log_debug("2");
	jmethodID mid = (*env)->GetMethodID(env, linkClass, "openDeclaration", "(Lnet/natpad/brilliance/chameleon/Cursor;)V");
	cat_log_debug("3");
	if (mid) {
		jobject jcursor = bri_cha_cursor_wo_as_java(env, cursor);
		(*env)->CallVoidMethod(env, priv->java_link, mid, jcursor);
	}
	brilliance_detach_java(priv->jvm);
}

static void l_format(DraContextEditor *dra_context_editor) {
	BriDraContextEditor *context_editor = BRI_DRA_CONTEXT_EDITOR(dra_context_editor);
	BriDraContextEditorPrivate *priv = bri_dra_context_editor_get_instance_private(context_editor);
	JNIEnv *env = brilliance_attach_java(priv->jvm);
	jclass linkClass = (*env)->GetObjectClass(env, priv->java_link);
	cat_log_debug("2");
	jmethodID mid = (*env)->GetMethodID(env, linkClass, "format", "()V");
	cat_log_debug("3");
	if (mid) {
		(*env)->CallVoidMethod(env, priv->java_link, mid);
	}
	brilliance_detach_java(priv->jvm);
}


static void l_context_insert_text(DraContextEditor *dra_context_editor, CatStringWo *text) {
	BriDraContextEditor *context_editor = BRI_DRA_CONTEXT_EDITOR(dra_context_editor);
	BriDraContextEditorPrivate *priv = bri_dra_context_editor_get_instance_private(context_editor);
	JNIEnv *env = brilliance_attach_java(priv->jvm);
	jclass linkClass = (*env)->GetObjectClass(env, priv->java_link);
	cat_log_debug("2");
	jmethodID mid = (*env)->GetMethodID(env, linkClass, "contextInsertText", "(Ljava/lang/String;)V");
	cat_log_debug("3");
	if (mid) {
		jstring t = (*env)->NewStringUTF(env, cat_string_wo_getchars(text));
		(*env)->CallVoidMethod(env, priv->java_link, mid, t);
	}
	brilliance_detach_java(priv->jvm);
}
