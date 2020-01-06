/*
   File:    briworrequest.c
   Project: brilliance-lib-jni
   Author:  Douwe Vos
   Date:    Sep 12, 2019
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

#include "briworrequest.h"
#include "../brilliance_native_helper.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "BriWorRequest"
#include <logging/catlog.h>

struct _BriWorRequestPrivate {
	JavaVM *jvm;
	jobject java_link;
};

G_DEFINE_TYPE_WITH_CODE(BriWorRequest, bri_wor_request, WOR_TYPE_REQUEST,  // @suppress("Unused static function")
	G_ADD_PRIVATE(BriWorRequest)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_run_request(WorRequest *request);

static void bri_wor_request_class_init(BriWorRequestClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	WorRequestClass *wor_class = WOR_REQUEST_CLASS(clazz);
	wor_class->runRequest = l_run_request;
}

static void bri_wor_request_init(BriWorRequest *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	BriWorRequest *instance = BRI_WOR_REQUEST(object);
	BriWorRequestPrivate *priv = bri_wor_request_get_instance_private(instance);
	G_OBJECT_CLASS(bri_wor_request_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(bri_wor_request_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


BriWorRequest *bri_wor_request_new(JNIEnv *env) {
	BriWorRequest *result = g_object_new(BRI_WOR_TYPE_REQUEST, NULL);
	cat_ref_anounce(result);
	BriWorRequestPrivate *priv = bri_wor_request_get_instance_private(result);
	priv->java_link = NULL;
	wor_request_construct((WorRequest *) result);
	(*env)->GetJavaVM(env, &(priv->jvm));
	return result;
}

void bri_wor_request_link(BriWorRequest *request, jobject java_link) {
	BriWorRequestPrivate *priv = bri_wor_request_get_instance_private(request);
	priv->java_link = java_link;
}


static void l_run_request(WorRequest *request) {
	BriWorRequest *wor_request = BRI_WOR_REQUEST(request);
	BriWorRequestPrivate *priv = bri_wor_request_get_instance_private(wor_request);
	JNIEnv *env = brilliance_attach_java(priv->jvm);

	jclass linkClass = (*env)->GetObjectClass(env, priv->java_link);
	jmethodID mid = (*env)->GetMethodID(env, linkClass, "runRequest", "()V");
	cat_log_debug("mid=%p", mid);
	(*env)->CallVoidMethod(env, priv->java_link, mid);
	if ((*env)->ExceptionCheck(env)) {
		cat_log_error("");
	}
	brilliance_detach_java(priv->jvm);
}







