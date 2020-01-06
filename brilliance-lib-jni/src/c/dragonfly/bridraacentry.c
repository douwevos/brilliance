/*
   File:    bridraacentry.c
   Project: brilliance-lib-jni
   Author:  Douwe Vos
   Date:    Dec 4, 2019
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

#include "bridraacentry.h"
#include "../brilliance_native_helper.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "BriDraAcEntry"
#include <logging/catlog.h>

struct _BriDraAcEntryPrivate {
	JavaVM *jvm;
	jobject java_link;
};

G_DEFINE_TYPE_WITH_CODE(BriDraAcEntry, bri_dra_ac_entry, DRA_TYPE_AC_ENTRY,  // @suppress("Unused static function")
	G_ADD_PRIVATE(BriDraAcEntry)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_run(DraAcEntry *entry, struct _DraAcContext *ac_context);

static void bri_dra_ac_entry_class_init(BriDraAcEntryClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	DraAcEntryClass *entry_class = DRA_AC_ENTRY_CLASS(clazz);
	entry_class->run = l_run;
}

static void bri_dra_ac_entry_init(BriDraAcEntry *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	BriDraAcEntry *instance = BRI_DRA_AC_ENTRY(object);
	BriDraAcEntryPrivate *priv = bri_dra_ac_entry_get_instance_private(instance);
	if (priv->java_link) {
		JNIEnv *env = brilliance_attach_java(priv->jvm);
		bri_dra_ac_entry_unlink(instance, env);
		priv->java_link = NULL;
		brilliance_detach_java(priv->jvm);
		cat_log_debug("unset_link");
	}

	G_OBJECT_CLASS(bri_dra_ac_entry_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(bri_dra_ac_entry_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


BriDraAcEntry *bri_dra_ac_entry_new(JNIEnv *env, CatStringWo *text) {
	BriDraAcEntry *result = g_object_new(BRI_TYPE_DRA_AC_ENTRY, NULL);
	cat_ref_anounce(result);
	BriDraAcEntryPrivate *priv = bri_dra_ac_entry_get_instance_private(result);
	dra_ac_entry_construct((DraAcEntry *) result, text);
	(*env)->GetJavaVM(env, &(priv->jvm));
	return result;
}

BriDraAcEntry *bri_dra_ac_entry_new_full(JNIEnv *env, CatStringWo *entry_text, CatStringWo *type_text, gboolean enabled, int color_code, int type_color_code)  {
	BriDraAcEntry *result = g_object_new(BRI_TYPE_DRA_AC_ENTRY, NULL);
	cat_ref_anounce(result);
	BriDraAcEntryPrivate *priv = bri_dra_ac_entry_get_instance_private(result);
	dra_ac_entry_construct_full((DraAcEntry *) result, entry_text, type_text, enabled, color_code, type_color_code);
	(*env)->GetJavaVM(env, &(priv->jvm));
	return result;
}


void bri_dra_ac_entry_link(BriDraAcEntry *entry, JNIEnv *env, jobject java_link) {
	BriDraAcEntryPrivate *priv = bri_dra_ac_entry_get_instance_private(entry);
	cat_log_debug("set_link");
	jobject g_java_link = (*env)->NewGlobalRef(env, java_link);
	priv->java_link = g_java_link;

}

void bri_dra_ac_entry_unlink(BriDraAcEntry *entry, JNIEnv *env) {
	BriDraAcEntryPrivate *priv = bri_dra_ac_entry_get_instance_private(entry);
	cat_log_debug("unset_link:%p", entry);
	if (priv->java_link) {
		(*env)->DeleteGlobalRef(env, priv->java_link);
		cat_log_debug("unset_link 2");
		priv->java_link = NULL;
	}
}


static void l_run(DraAcEntry *entry, struct _DraAcContext *ac_context) {
	BriDraAcEntryPrivate *priv = bri_dra_ac_entry_get_instance_private((BriDraAcEntry *) entry);

	JNIEnv *env = brilliance_attach_java(priv->jvm);

	jclass context_cls = (*env)->FindClass(env, "net/natpad/brilliance/dragonfly/AcContext");
	cat_log_debug("create_req:5: context_cls=%p", context_cls);
	jmethodID mi_constructor = (*env)->GetMethodID(env, context_cls, "<init>", "(J)V");
	jobject ac_context_obj = (*env)->NewObject(env, context_cls, mi_constructor, (jlong) cat_ref_ptr(ac_context));





	cat_log_debug("xx:1:env=%p, priv->java_link=%p", env, priv->java_link);
	jclass linkClass = (*env)->GetObjectClass(env, priv->java_link);
	cat_log_debug("xx:2:linkClass=%p",linkClass);
	jmethodID mi_run = (*env)->GetMethodID(env, linkClass, "run", "(Lnet/natpad/brilliance/dragonfly/AcContext;)V");

	cat_log_debug("mi_run=%p", mi_run);

	(*env)->CallVoidMethod(env, priv->java_link, mi_run, (jlong) ac_context_obj);
	if ((*env)->ExceptionCheck(env)) {
		cat_log_error("in RunAugment");
		brilliance_detach_java(priv->jvm);
		return;
	}

//	cat_log_debug("DeleteGlobalRef");
//	bri_dra_augment_request_unlink(request, env);
	cat_log_debug("do detach");
	brilliance_detach_java(priv->jvm);
//	cat_log_debug("detach done: rc=%d", ((GObject *) request)->ref_count);
}
