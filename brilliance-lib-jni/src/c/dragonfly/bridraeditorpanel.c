/*
   File:    bridraeditorpanel.c
   Project: brilliance-lib-jni
   Author:  Douwe Vos
   Date:    Dec 10, 2019
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

#include "bridraeditorpanel.h"
#include "bridraconnectorrequestfactory.h"
#include "../brilliance_native_helper.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "BriDraEditorPanel"
#include <logging/catlog.h>

struct _BriDraEditorPanelPrivate {
	JavaVM *jvm;
	jweak weak_link;
	CatStringWo *cached_name;
};

G_DEFINE_TYPE_WITH_CODE(BriDraEditorPanel, bri_dra_editor_panel, DRA_TYPE_EDITOR_PANEL,  // @suppress("Unused static function")
	G_ADD_PRIVATE(BriDraEditorPanel)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static DraEditor *l_init_editor(DraEditorPanel *editor_panel, ChaDocument *document, DraConnectorMap *connector_map, DraIConnectorRequestFactory *request_factory, WorService *wor_service);
static const CatStringWo *l_get_name(const DraEditorPanel *editor);


static void bri_dra_editor_panel_class_init(BriDraEditorPanelClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	DraEditorPanelClass *cls = DRA_EDITOR_PANEL_CLASS(clazz);
	cls->initEditor = l_init_editor;
	cls->getName = l_get_name;

}

static void bri_dra_editor_panel_init(BriDraEditorPanel *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	BriDraEditorPanel *instance = BRI_DRA_EDITOR_PANEL(object);
	BriDraEditorPanelPrivate *priv = bri_dra_editor_panel_get_instance_private(instance);
	G_OBJECT_CLASS(bri_dra_editor_panel_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(bri_dra_editor_panel_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


BriDraEditorPanel *bri_dra_editor_panel_new() {
	BriDraEditorPanel *result = g_object_new(BRI_TYPE_DRA_EDITOR_PANEL, NULL);
	cat_ref_anounce(result);
	return result;
}

//BriDraEditorPanel *bri_dra_editor_panel_new(JNIEnv *env, LeaIPanelOwner *panel_owner, ChaDocument *document, DraIConnectorRequestFactory *request_factory) {
//	BriDraEditorPanel *result = g_object_new(BRI_TYPE_DRA_EDITOR_PANEL, NULL);
//	cat_ref_anounce(result);
//	cat_log_debug("auw");
//	BriDraEditorPanelPrivate *priv = bri_dra_editor_panel_get_instance_private(result);
//	(*env)->GetJavaVM(env, &(priv->jvm));
//	cat_log_debug("auwfddf");
//	dra_editor_panel_construct((DraEditorPanel *) result, panel_owner, document, request_factory);
//	cat_log_debug("new here");
//	return result;
//}

void bri_dra_editor_panel_construct(BriDraEditorPanel *editor_panel, JNIEnv *env, jobject editor_panel_obj, LeaIPanelOwner *panel_owner, ChaDocument *document, DraIConnectorRequestFactory *request_factory) {
	BriDraEditorPanelPrivate *priv = bri_dra_editor_panel_get_instance_private(editor_panel);
	cat_log_debug("linking");
	priv->weak_link = (*env)->NewWeakGlobalRef(env, editor_panel_obj);
	(*env)->GetJavaVM(env, &(priv->jvm));
	priv->cached_name = NULL;
	dra_editor_panel_construct((DraEditorPanel *) editor_panel, panel_owner, document, request_factory);
}


void bri_dra_editor_panel_link(BriDraEditorPanel *editor_panel, JNIEnv *env, jobject java_link) {
	BriDraEditorPanelPrivate *priv = bri_dra_editor_panel_get_instance_private(editor_panel);
	cat_log_debug("linking");
	priv->weak_link = (*env)->NewWeakGlobalRef(env, java_link);
	cat_log_debug("priv->weak_link=%p", priv->weak_link);
}

static DraEditor *l_init_editor(DraEditorPanel *editor_panel, ChaDocument *document, DraConnectorMap *connector_map, DraIConnectorRequestFactory *request_factory, WorService *wor_service) {
	BriDraEditorPanelPrivate *priv = bri_dra_editor_panel_get_instance_private((BriDraEditorPanel *) editor_panel);
	cat_log_debug("start");
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

	jobject jconnector_map = NULL;
	if (connector_map) {
		jclass connector_map_cls = (*env)->FindClass(env, "net/natpad/brilliance/dragonfly/ConnectorMap");
		jmethodID mi_connector_map_constructor = (*env)->GetMethodID(env, connector_map_cls, "<init>", "(J)V");
		jconnector_map = (*env)->NewObject(env, connector_map_cls, mi_connector_map_constructor, (jlong) connector_map);
		g_object_ref(connector_map);
	}
	cat_log_debug("jconnector_map=%p", jconnector_map);

	jobject jrequest_factory = NULL;
	if (request_factory) {
		BriDraConnectorRequestFactory *bri_req_factory = BRI_DRA_CONNECTOR_REQUEST_FACTORY(request_factory);
		jrequest_factory = bri_dra_connector_request_factory_get_link_ref(bri_req_factory, env);
	}
	cat_log_debug("jrequest_factory=%p", jrequest_factory);

	jobject jwor_service = NULL;
	if (wor_service) {
		jclass wor_service_cls = (*env)->FindClass(env, "net/natpad/brilliance/worm/WorkerService");
		jmethodID mi_wor_service_constructor = (*env)->GetMethodID(env, wor_service_cls, "<init>", "(J)V");
		jwor_service = (*env)->NewObject(env, wor_service_cls, mi_wor_service_constructor, (jlong) wor_service);
		g_object_ref(wor_service);
	}
	cat_log_debug("jwor_service=%p", jwor_service);


	jmethodID mi_init_editor = (*env)->GetMethodID(env, obj_class, "initEditor", "(Lnet/natpad/brilliance/chameleon/Document;Lnet/natpad/brilliance/dragonfly/ConnectorMap;Lnet/natpad/brilliance/dragonfly/IConnectorRequestFactory;Lnet/natpad/brilliance/worm/WorkerService;)Lnet/natpad/brilliance/dragonfly/DragonflyEditor;");
	cat_log_debug("mi_init_editor=%p", mi_init_editor);

	jobject o = (*env)->CallObjectMethod(env, local_link, mi_init_editor, jdocument, jconnector_map, jrequest_factory, jwor_service);
	cat_log_debug("mi_init_editor=%p", mi_init_editor);

	DraEditor *result = brilliance_as_object(env, o);


	return result;
}


static const CatStringWo *l_get_name(const DraEditorPanel *editor_panel) {
	BriDraEditorPanelPrivate *priv = bri_dra_editor_panel_get_instance_private((BriDraEditorPanel *) editor_panel);
	cat_log_debug("start");
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


	jmethodID mi_init_editor = (*env)->GetMethodID(env, obj_class, "getName", "()Ljava/lang/String;");
	cat_log_debug("mi_init_editor=%p", mi_init_editor);

	jobject o = (*env)->CallObjectMethod(env, local_link, mi_init_editor);
	cat_log_debug("mi_init_editor=%p", mi_init_editor);

	CatStringWo *result = brilliance_from_java_string(env, (jstring) o);
	cat_unref_ptr(priv->cached_name);
	priv->cached_name = result;
	return result;
}
