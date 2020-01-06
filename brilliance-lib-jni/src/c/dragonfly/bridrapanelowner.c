/*
   File:    bridrapanelowner.c
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

#include "bridrapanelowner.h"
#include "../brilliance_native_helper.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "BriDraPanelOwner"
#include <logging/catlog.h>

struct _BriDraPanelOwnerPrivate {
	JavaVM *jvm;
	jobject java_link;
};

G_DEFINE_TYPE_WITH_CODE(BriDraPanelOwner, bri_dra_panel_owner, DRA_TYPE_PANEL_OWNER,  // @suppress("Unused static function")
	G_ADD_PRIVATE(BriDraPanelOwner)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void bri_dra_panel_owner_class_init(BriDraPanelOwnerClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void bri_dra_panel_owner_init(BriDraPanelOwner *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	BriDraPanelOwner *instance = BRI_DRA_PANEL_OWNER(object);
//	BriDraPanelOwnerPrivate *priv = bri_dra_panel_owner_get_instance_private(instance);
	G_OBJECT_CLASS(bri_dra_panel_owner_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(bri_dra_panel_owner_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


BriDraPanelOwner *bri_dra_panel_owner_new(JNIEnv *env, LeaFrame *frame, WorService *service) {
	BriDraPanelOwner *result = g_object_new(BRI_DRA_TYPE_PANEL_OWNER, NULL);
	cat_ref_anounce(result);
	BriDraPanelOwnerPrivate *priv = bri_dra_panel_owner_get_instance_private(result);
	dra_panel_owner_construct((DraPanelOwner *) result, frame, service);
	(*env)->GetJavaVM(env, &(priv->jvm));
	return result;
}

void bri_dra_panel_owner_link(BriDraPanelOwner *request, jobject java_link) {
	BriDraPanelOwnerPrivate *priv = bri_dra_panel_owner_get_instance_private(request);
	priv->java_link = java_link;
}
