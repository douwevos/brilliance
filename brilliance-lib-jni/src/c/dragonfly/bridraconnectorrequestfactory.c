/*
   File:    bridraconnectorrequestfactory.c
   Project: brilliance-lib-jni
   Author:  Douwe Vos
   Date:    Sep 15, 2019
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

#include "bridraconnectorrequestfactory.h"
#include <dragonfly.h>
#include "../brilliance_native_helper.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "BriDraConnectorRequestFactory"
#include <logging/catlog.h>

struct _BriDraConnectorRequestFactoryPrivate {
	JavaVM *jvm;
	jobject factory_weak;
};

static void l_connector_requestor_factory_iface_init(DraIConnectorRequestFactoryInterface *iface);

G_DEFINE_TYPE_WITH_CODE(BriDraConnectorRequestFactory, bri_dra_connector_request_factory, G_TYPE_OBJECT,  // @suppress("Unused static function")
	G_ADD_PRIVATE(BriDraConnectorRequestFactory)
	G_IMPLEMENT_INTERFACE(DRA_TYPE_ICONNECTOR_REQUEST_FACTORY, l_connector_requestor_factory_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void bri_dra_connector_request_factory_class_init(BriDraConnectorRequestFactoryClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void bri_dra_connector_request_factory_init(BriDraConnectorRequestFactory *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	BriDraConnectorRequestFactory *instance = BRI_DRA_CONNECTOR_REQUEST_FACTORY(object);
//	BriDraConnectorRequestFactoryPrivate *priv = bri_dra_connector_request_factory_get_instance_private(instance);
	G_OBJECT_CLASS(bri_dra_connector_request_factory_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(bri_dra_connector_request_factory_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


BriDraConnectorRequestFactory *bri_dra_connector_request_factory_new(JNIEnv *env, jobject factory_java) {
	BriDraConnectorRequestFactory *result = g_object_new(BRI_DRA_TYPE_CONNECTOR_REQUEST_FACTORY, NULL);
	cat_ref_anounce(result);
	BriDraConnectorRequestFactoryPrivate *priv = bri_dra_connector_request_factory_get_instance_private(result);
	(*env)->GetJavaVM(env, &(priv->jvm));
	priv->factory_weak = (*env)->NewGlobalRef(env, factory_java);
	return result;
}

jobject bri_dra_connector_request_factory_get_link_ref(BriDraConnectorRequestFactory *factory, JNIEnv *env) {
	BriDraConnectorRequestFactoryPrivate *priv = bri_dra_connector_request_factory_get_instance_private(factory);
	return (*env)->NewLocalRef(env, priv->factory_weak);
}

static DraAugmentRequest *l_factory_create_request(DraIConnectorRequestFactory *content_provider, ChaDocument *document, ChaRevisionWo *a_new_revision) {
	cat_log_debug("content_provider:%p", content_provider);
	BriDraConnectorRequestFactory *factory = BRI_DRA_CONNECTOR_REQUEST_FACTORY(content_provider);
	BriDraConnectorRequestFactoryPrivate *priv = bri_dra_connector_request_factory_get_instance_private(factory);

	DraAugmentRequest *result = NULL;

	cat_log_debug("create_req:1");
	JNIEnv *env = brilliance_attach_java(priv->jvm);
	cat_log_debug("create_req:2");
//	jobject obj = (*env)->NewLocalRef(env, priv->factory_weak);
	jobject obj = priv->factory_weak;
	if (obj != NULL) {
		jclass obj_class = (*env)->GetObjectClass(env, obj);
		if ((*env)->ExceptionCheck(env)) {
			cat_log_error("in GetObjectClass");
			brilliance_detach_java(priv->jvm);
			return NULL;
		}
		cat_log_debug("create_req:3:obj_class=%p", obj_class);
		jmethodID mid = (*env)->GetMethodID(env, obj_class, "createRequest", "(Lnet/natpad/brilliance/chameleon/Document;Lnet/natpad/brilliance/chameleon/Revision;)Lnet/natpad/brilliance/dragonfly/AugmentRequest;");
		cat_log_debug("create_req:4:moid=%p", mid);

//		cat_log_debug("go:4");
//		jmethodID mid2 = (*env)->GetMethodID(env, obj_class, "getSlotKey", "()Lnet/natpad/brilliance/caterpillar/StringWo;");
//		cat_log_debug("mid2=%p", mid2);


		jclass document_cls = (*env)->FindClass(env, "net/natpad/brilliance/chameleon/Document");
		cat_log_debug("create_req:5: document_cls=%p", document_cls);
		jmethodID mod_d = (*env)->GetMethodID(env, document_cls, "<init>", "(J)V");
		cat_log_debug("create_req:6:mod_d=%p", mod_d);
		jobject jdocument = (*env)->NewObject(env, document_cls, mod_d, (jlong) document);
		cat_log_debug("create_req:7,document=%d",((GObject *) document)->ref_count);
		g_object_ref(document);

		jclass revision_cls = (*env)->FindClass(env, "net/natpad/brilliance/chameleon/Revision");
		cat_log_debug("create_req:8:%p", revision_cls);
		jmethodID mod_r = (*env)->GetMethodID(env, revision_cls, "<init>", "(J)V");
		cat_log_debug("create_req:9:%p", mod_r);
		jobject jrevision = (*env)->NewObject(env, revision_cls, mod_r, (jlong) a_new_revision);
		cat_log_debug("create_req:10:%p", jrevision);
		cat_ref_ptr(a_new_revision);

		jobject o = (*env)->CallObjectMethod(env, obj, mid, jdocument, jrevision);
		if ((*env)->ExceptionCheck(env)) {
			cat_log_error("in CallObjectMethod");
			g_object_unref(document);
			g_object_unref(a_new_revision);
			return NULL;
		}
		cat_log_debug("create_req:11:%o", o);
		result = brilliance_as_object(env, o);
		cat_log_debug("create_req:12:%o, rc=%d", result, ((GObject *) result)->ref_count);

		g_object_ref(result);
//		(*env)->DeleteLocalRef(env, obj);
	}
	brilliance_detach_java(priv->jvm);
	cat_log_debug("req:%o", result);

	return result;
}

CatStringWo *l_factory_get_slot_key(DraIConnectorRequestFactory *content_provider) {
	BriDraConnectorRequestFactory *factory = BRI_DRA_CONNECTOR_REQUEST_FACTORY(content_provider);
	BriDraConnectorRequestFactoryPrivate *priv = bri_dra_connector_request_factory_get_instance_private(factory);
	CatStringWo *result = NULL;

	cat_log_debug("go:1");

	JNIEnv *env = brilliance_attach_java(priv->jvm);
	cat_log_debug("go:22:weak=%p", priv->factory_weak);
//	jobject obj = (*env)->NewGlobalRef(env, priv->factory_weak);
	jobject obj = priv->factory_weak;
	cat_log_debug("go:2");
	if (obj != NULL) {
		cat_log_debug("go:3");
		jclass obj_class = (*env)->GetObjectClass(env, obj);
		cat_log_debug("go:4");
		jmethodID mid = (*env)->GetMethodID(env, obj_class, "getSlotKey", "()Lnet/natpad/brilliance/caterpillar/StringWo;");
		cat_log_debug("go:5: mid=%p", mid);
		jobject o = (*env)->CallObjectMethod(env, obj, mid);
		if ((*env)->ExceptionCheck(env)) {
			cat_log_error("exception in calling getSlotKey detected");
			brilliance_detach_java(priv->jvm);
			return NULL;
		}
		cat_log_debug("go:6: o= %p", o);
		result = brilliance_as_object(env, o);
		cat_log_debug("go:7");

//		(*env)->DeleteLocalRef(env, obj);
		cat_log_debug("go:8");
		g_object_ref(result);
	}
	brilliance_detach_java(priv->jvm);
	return result;
}


static void l_connector_requestor_factory_iface_init(DraIConnectorRequestFactoryInterface *iface) {
	iface->createRequest = l_factory_create_request;
	iface->getSlotKey = l_factory_get_slot_key;
}
