/*
   File:    brileapanelowner.c
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

#include "brileapanelowner.h"
#include <leafhopper.h>
#include "../brilliance_native_helper.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "BriLeaPanelOwner"
#include <logging/catlog.h>

struct _BriLeaPanelOwnerPrivate {
	JavaVM *jvm;
	jobject factory_weak;
};

static void l_panel_owner_factory_iface_init(LeaIPanelOwnerInterface *iface);

G_DEFINE_TYPE_WITH_CODE(BriLeaPanelOwner, bri_lea_panel_owner, G_TYPE_OBJECT,  // @suppress("Unused static function")
	G_ADD_PRIVATE(BriLeaPanelOwner)
	G_IMPLEMENT_INTERFACE(LEA_TYPE_IPANEL_OWNER, l_panel_owner_factory_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void bri_lea_panel_owner_class_init(BriLeaPanelOwnerClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void bri_lea_panel_owner_init(BriLeaPanelOwner *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	BriLeaPanelOwner *instance = BRI_LEA_PANEL_OWNER(object);
//	BriLeaPanelOwnerPrivate *priv = bri_lea_panel_owner_get_instance_private(instance);
	G_OBJECT_CLASS(bri_lea_panel_owner_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(bri_lea_panel_owner_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


BriLeaPanelOwner *bri_lea_panel_owner_new(JNIEnv *env, jobject panel_owner_java) {
	BriLeaPanelOwner *result = g_object_new(BRI_LEA_TYPE_PANEL_OWNER, NULL);
	cat_ref_anounce(result);
	BriLeaPanelOwnerPrivate *priv = bri_lea_panel_owner_get_instance_private(result);
	(*env)->GetJavaVM(env, &(priv->jvm));
	priv->factory_weak = (*env)->NewGlobalRef(env, panel_owner_java);
	return result;
}


static void l_panel_owner_add_panel(LeaIPanelOwner *panel_owner, struct _LeaPanel *panel) {
	BriLeaPanelOwner *factory = BRI_LEA_PANEL_OWNER(panel_owner);
	BriLeaPanelOwnerPrivate *priv = bri_lea_panel_owner_get_instance_private(factory);

	cat_log_debug("create_req:1");
	JNIEnv *env = brilliance_attach_java(priv->jvm);
	cat_log_debug("create_req:2");
//	jobject obj = (*env)->NewLocalRef(env, priv->factory_weak);
	jobject obj = priv->factory_weak;
	if (obj != NULL) {
		jclass obj_class = (*env)->GetObjectClass(env, obj);
		cat_log_debug("go:4");
		jmethodID mid = (*env)->GetMethodID(env, obj_class, "addPanel", "(Lnet/natpad/brilliance/leafhopper/Panel;)V");
		cat_log_debug("go:5: mid=%p", mid);
		(*env)->CallVoidMethod(env, obj, mid, panel);
		if ((*env)->ExceptionCheck(env)) {
			cat_log_error("exception in PanelOwner.addPanel");
			brilliance_detach_java(priv->jvm);
			return;
		}
		cat_log_debug("go:6:");
	}
	brilliance_detach_java(priv->jvm);
}

static void l_panel_owner_remove_panel(LeaIPanelOwner *panel_owner, struct _LeaPanel *panel) {
	cat_log_error("to be implemented");
}

static LeaKeyContext *l_panel_owner_get_key_context(LeaIPanelOwner *panel_owner) {
	cat_log_error("to be implemented");
	return NULL;
}

GtkWidget *l_panel_owner_get_status_bar_content(LeaIPanelOwner *panel_owner) {
	cat_log_error("to be implemented");
	return NULL;
}

gboolean l_panel_owner_panel_closing(LeaIPanelOwner *panel_owner, struct _LeaPanel *panel) {
	cat_log_error("to be implemented");
	return TRUE;
}


static void l_panel_owner_factory_iface_init(LeaIPanelOwnerInterface *iface) {
	iface->addPanel = l_panel_owner_add_panel;
	iface->removePanel = l_panel_owner_remove_panel;
	iface->getKeyContext = l_panel_owner_get_key_context;
	iface->getStatusBarContent = l_panel_owner_get_status_bar_content;
	iface->panel_closing = l_panel_owner_panel_closing;
}
