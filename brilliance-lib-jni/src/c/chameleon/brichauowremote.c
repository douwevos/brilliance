/*
   File:    brichauowremote.c
   Project: brilliance-lib-jni
   Author:  Douwe Vos
   Date:    Jan 2, 2020
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

#include "brichauowremote.h"
#include "brichadocumentview.h"
#include "../brilliance_native_helper.h"
#include "../dragonfly/bridraeditor.h"
#include "../dragonfly/bridradocumentview.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "BriChaUowRemote"
#include <logging/catlog.h>

struct _BriChaUowRemotePrivate {
	JavaVM *jvm;
	jweak weak_link;
};

G_DEFINE_TYPE_WITH_CODE(BriChaUowRemote, bri_cha_uow_remote, CHA_TYPE_UOW,  // @suppress("Unused static function")
	G_ADD_PRIVATE(BriChaUowRemote)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_uow_run(ChaUow *self, struct _ChaEditor *editor, ChaDocumentView *document_view);

static void bri_cha_uow_remote_class_init(BriChaUowRemoteClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	ChaUowClass *uow_class = CHA_UOW_CLASS(clazz);
	uow_class->run = l_uow_run;
}

static void bri_cha_uow_remote_init(BriChaUowRemote *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	BriChaUowRemote *instance = BRI_CHA_UOW_REMOTE(object);
//	BriChaUowRemotePrivate *priv = bri_cha_uow_remote_get_instance_private(instance);
	G_OBJECT_CLASS(bri_cha_uow_remote_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(bri_cha_uow_remote_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


BriChaUowRemote *bri_cha_uow_remote_new() {
	BriChaUowRemote *result = g_object_new(BRI_TYPE_CHA_UOW_REMOTE, NULL);
	cat_ref_anounce(result);
	BriChaUowRemotePrivate *priv = bri_cha_uow_remote_get_instance_private(result);
	return result;
}

void bri_cha_uow_remote_construct(BriChaUowRemote *uow_remote, JNIEnv *env, jobject uow_remote_obj, jboolean is_read_only) {
	BriChaUowRemotePrivate *priv = bri_cha_uow_remote_get_instance_private(uow_remote);
	(*env)->GetJavaVM(env, &(priv->jvm));
	cat_log_debug("construct:%p, vm=%p, priv=%p", uow_remote, priv->jvm, priv);
	priv->weak_link = (*env)->NewWeakGlobalRef(env, uow_remote_obj);
	cha_uow_construct((ChaUow *) uow_remote, is_read_only==JNI_TRUE);
}


static void l_uow_run(ChaUow *self, struct _ChaEditor *editor, ChaDocumentView *document_view) {
	BriChaUowRemotePrivate *priv = bri_cha_uow_remote_get_instance_private(BRI_CHA_UOW_REMOTE(self));
	JNIEnv* env = brilliance_attach_java(priv->jvm);
	cat_log_debug("attached");

	jobject jeditor;
	if (BRI_IS_DRA_EDITOR(editor)) {
		cat_log_debug("get link");
		jeditor = bri_dra_editor_get_link((BriDraEditor *) editor, env);
	} else {
		cat_log_error("in GetObjectClass");
		brilliance_detach_java(priv->jvm);
		return;
	}
	cat_log_debug("jeditor=%p", jeditor);

	jobject jdocument_view;
	cat_log_debug("document_view=%O", document_view);
	if (BRI_IS_CHA_DOCUMENT_VIEW(document_view)) {
		jdocument_view = bri_cha_document_view_get_link((BriChaDocumentView *) document_view, env);
	} else if (BRI_IS_DRA_DOCUMENT_VIEW(document_view)) {
		jdocument_view = bri_dra_document_view_get_link((BriDraDocumentView *) document_view, env);
	} else {
		cat_log_error("in GetObjectClass");
		brilliance_detach_java(priv->jvm);
		return;
	}


	jobject local_link = (*env)->NewLocalRef(env, priv->weak_link);
	cat_log_debug("local_link=%p", local_link);
	jclass obj_class = (*env)->GetObjectClass(env, local_link);
	cat_log_debug("obj_class=%p", obj_class);
	if ((*env)->ExceptionCheck(env)) {
		cat_log_error("in GetObjectClass");
		brilliance_detach_java(priv->jvm);
		return;
	}

	jmethodID mi_run = (*env)->GetMethodID(env, obj_class, "run", "(Lnet/natpad/brilliance/chameleon/ChameleonEditor;Lnet/natpad/brilliance/chameleon/DocumentView;)V");
	cat_log_debug("mi_run=%p", mi_run);

	(*env)->CallVoidMethod(env, local_link, mi_run, jeditor, jdocument_view);
	brilliance_detach_java(priv->jvm);

}
