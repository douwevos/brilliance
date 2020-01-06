/*
   File:    bridraaugmentrequest.c
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

#include "bridraaugmentrequest.h"
#include "../brilliance_native_helper.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "BriDraAugmentRequest"
#include <logging/catlog.h>

struct _BriDraAugmentRequestPrivate {
	JavaVM *jvm;
	jobject java_link;
};

G_DEFINE_TYPE_WITH_CODE(BriDraAugmentRequest, bri_dra_augment_request, DRA_TYPE_AUGMENT_REQUEST,  // @suppress("Unused static function")
	G_ADD_PRIVATE(BriDraAugmentRequest)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static gboolean l_run_augment(DraAugmentRequest *request, ChaRevisionWo *a_revision, DraKeywordPrinter *keyword_printer, DraKeywordPrinter *line_tag_printer);

static void bri_dra_augment_request_class_init(BriDraAugmentRequestClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	DraAugmentRequestClass *req_class = DRA_AUGMENT_REQUEST_CLASS(clazz);
	req_class->runAugment = l_run_augment;
}

static void bri_dra_augment_request_init(BriDraAugmentRequest *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	BriDraAugmentRequest *instance = BRI_DRA_AUGMENT_REQUEST(object);
	BriDraAugmentRequestPrivate *priv = bri_dra_augment_request_get_instance_private(instance);
	if (priv->java_link) {
		JNIEnv *env = brilliance_attach_java(priv->jvm);
		(*env)->DeleteGlobalRef(env, priv->java_link);
		priv->java_link = NULL;
		brilliance_detach_java(priv->jvm);
		cat_log_debug("unset_link");
	}

	G_OBJECT_CLASS(bri_dra_augment_request_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(bri_dra_augment_request_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


BriDraAugmentRequest *bri_dra_augment_request_new(JNIEnv *env, ChaDocument *document, ChaRevisionWo *revision, CatStringWo *slot_key) {
	BriDraAugmentRequest *result = g_object_new(BRI_DRA_TYPE_AUGMENT_REQUEST, NULL);
	cat_ref_anounce(result);
	BriDraAugmentRequestPrivate *priv = bri_dra_augment_request_get_instance_private(result);
	dra_augment_request_construct((DraAugmentRequest *) result, document, revision, slot_key);
	(*env)->GetJavaVM(env, &(priv->jvm));
	return result;
}

void bri_dra_augment_request_link(BriDraAugmentRequest *request, jobject java_link) {
	BriDraAugmentRequestPrivate *priv = bri_dra_augment_request_get_instance_private(request);
	cat_log_debug("set_link");
	priv->java_link = java_link;
}

void bri_dra_augment_request_unlink(BriDraAugmentRequest *request, JNIEnv *env) {
	BriDraAugmentRequestPrivate *priv = bri_dra_augment_request_get_instance_private(request);
	cat_log_debug("unset_link:%p", request);
	if (priv->java_link) {
		(*env)->DeleteGlobalRef(env, priv->java_link);
		cat_log_debug("unset_link 2");
		priv->java_link = NULL;
	}
}


static gboolean l_run_augment(DraAugmentRequest *request, ChaRevisionWo *a_revision, DraKeywordPrinter *keyword_printer, DraKeywordPrinter *line_tag_printer) {
	BriDraAugmentRequest *aug_request = BRI_DRA_AUGMENT_REQUEST(request);
	BriDraAugmentRequestPrivate *priv = bri_dra_augment_request_get_instance_private(aug_request);
	JNIEnv *env = brilliance_attach_java(priv->jvm);

	cat_log_debug("xx:1:env=%p, priv->java_link=%p", env, priv->java_link);
	jclass linkClass = (*env)->GetObjectClass(env, priv->java_link);
	cat_log_debug("xx:2:linkClass=%p",linkClass);
	jmethodID mid = (*env)->GetMethodID(env, linkClass, "runAugmentLink", "(JJJ)Z");
	cat_log_debug("mid=%p", mid);
	cat_ref_ptr(a_revision);
	cat_ref_ptr(keyword_printer);
	cat_ref_ptr(line_tag_printer);

	jboolean result = (*env)->CallBooleanMethod(env, priv->java_link, mid, (jlong) a_revision, (jlong) keyword_printer, (jlong) line_tag_printer);
	if ((*env)->ExceptionCheck(env)) {
		cat_log_error("in RunAugment");
		brilliance_detach_java(priv->jvm);
		return FALSE;
	}

	cat_log_debug("DeleteGlobalRef");
	bri_dra_augment_request_unlink(request, env);
	cat_log_debug("do detach");
	brilliance_detach_java(priv->jvm);
	cat_log_debug("detach done: rc=%d", ((GObject *) request)->ref_count);
	return result==JNI_TRUE ? TRUE : FALSE;
}
