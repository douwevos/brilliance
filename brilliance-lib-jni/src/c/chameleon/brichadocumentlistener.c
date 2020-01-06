/*
   File:    brichadocumentlistener.c
   Project: brilliance-lib-jni
   Author:  Douwe Vos
   Date:    Sep 10, 2019
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

#include "brichadocumentlistener.h"
#include "../brilliance_native_helper.h"
#include <chameleon.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "BriChaDocumentListener"
#include <logging/catlog.h>

struct _BriChaDocumentListenerPrivate {
	JNIEnv *env;
	JavaVM *jvm;

	jobject hook;
};

static void bra_cha_document_listener_iface_init(ChaIDocumentListenerInterface *iface);

G_DEFINE_TYPE_WITH_CODE(BriChaDocumentListener, bri_cha_document_listener, G_TYPE_OBJECT,  // @suppress("Unused static function")
	G_ADD_PRIVATE(BriChaDocumentListener)
	G_IMPLEMENT_INTERFACE(CHA_TYPE_IDOCUMENT_LISTENER, bra_cha_document_listener_iface_init);
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void bri_cha_document_listener_class_init(BriChaDocumentListenerClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void bri_cha_document_listener_init(BriChaDocumentListener *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	BriChaDocumentListener *instance = BRI_CHA_DOCUMENT_LISTENER(object);
	BriChaDocumentListenerPrivate *priv = bri_cha_document_listener_get_instance_private(instance);
	if (priv->hook) {
		JNIEnv *env = brilliance_attach_java(priv->jvm);
		(*env)->DeleteGlobalRef(env, priv->hook);
		priv->hook = NULL;
		brilliance_detach_java(priv->jvm);

	}
	G_OBJECT_CLASS(bri_cha_document_listener_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(bri_cha_document_listener_parent_class)->finalize(object);

	cat_log_detail("finalized:%p", object);
}


BriChaDocumentListener *bri_cha_document_listener_new(JNIEnv *env, jobject hook) {
	BriChaDocumentListener *result = g_object_new(BRI_CHA_TYPE_DOCUMENT_LISTENER, NULL);
	cat_ref_anounce(result);
	BriChaDocumentListenerPrivate *priv = bri_cha_document_listener_get_instance_private(result);
	(*env)->GetJavaVM(env, &(priv->jvm));
	priv->hook = (*env)->NewGlobalRef(env, hook);
//	priv->hook = (*env)->NewLocalRef(env, hook);
//	priv->hook = hook;
	return result;
}

static void l_on_new_revision(ChaIDocumentListener *self, ChaRevisionWo *a_new_revision) {
	BriChaDocumentListener *listener = BRI_CHA_DOCUMENT_LISTENER(self);
	BriChaDocumentListenerPrivate *priv = bri_cha_document_listener_get_instance_private(listener);
	cat_log_debug("gogog:hook=%p", priv->hook);
	JNIEnv *env = brilliance_attach_java(priv->jvm);
	cat_log_debug("env=%p", env);
	jclass hookeClass = (*env)->GetObjectClass(env, priv->hook);

	cat_log_debug("hookeClass=%p", hookeClass);
	jmethodID mid = (*env)->GetMethodID(env, hookeClass, "onNewRevision", "(J)V");
	cat_log_debug("mid=%p", mid);
	cat_ref_ptr(a_new_revision);
	(*env)->CallVoidMethod(env, priv->hook, mid, (jlong) a_new_revision);
	if ((*env)->ExceptionCheck(env)) {
		cat_log_error("");
	}
	brilliance_detach_java(priv->jvm);
}


static void bra_cha_document_listener_iface_init(ChaIDocumentListenerInterface *iface) {
	iface->onNewRevision = l_on_new_revision;
}

