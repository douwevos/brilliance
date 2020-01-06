/*
   File:    net_natpad_brilliance_leafhopper_Leafhopper.c
   Project: brilliance-lib-jni
   Author:  Douwe Vos
   Date:    Sep 8, 2019
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

#include <jni.h>
#include "brilliance_native_helper.h"
#include "net_natpad_brilliance_leafhopper_Leafhopper.h"
#include <caterpillar.h>
#include <leafhopper.h>
#include <chameleon.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "BrillianceLeafhopper"
#include <logging/catlog.h>

/*
 * Class:     net_natpad_brilliance_leafhopper_Leafhopper
 * Method:    getSurfaceHandler
 * Signature: (Lnet/natpad/brilliance/leafhopper/Frame;)J
 */
JNIEXPORT jlong JNICALL Java_net_natpad_brilliance_leafhopper_Leafhopper_getSurfaceHandler
  (JNIEnv *env , jclass cls, jobject frame_obj) {
	LeaFrame *frame = brilliance_as_object(env, frame_obj);
	LeaSurfaceHandler *result = lea_frame_get_surface_handler(frame);
	return (jlong) cat_ref_ptr(result);
}

/*
 * Class:     net_natpad_brilliance_leafhopper_Leafhopper
 * Method:    createSurface
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_net_natpad_brilliance_leafhopper_Leafhopper_createSurface(JNIEnv *env, jclass cls) {
	return (jlong) lea_surface_new();
}

JNIEXPORT jlong JNICALL Java_net_natpad_brilliance_leafhopper_Leafhopper_getRoot
  (JNIEnv *env, jclass cls, jobject surface_handler_obj) {
	LeaSurfaceHandler *surface_handler = brilliance_as_object(env, surface_handler_obj);
	return (jlong) cat_ref_ptr(lea_surface_handler_get_root(surface_handler));
}

/*
 * Class:     net_natpad_brilliance_leafhopper_Leafhopper
 * Method:    add
 * Signature: (Lnet/natpad/brilliance/leafhopper/SurfaceBox;Lnet/natpad/brilliance/leafhopper/IRegion;II)V
 */
JNIEXPORT void JNICALL Java_net_natpad_brilliance_leafhopper_Leafhopper_add
  (JNIEnv *env, jclass cls, jobject surface_box_obj, jobject region_obj, jint index, jint weight) {
	LeaSurfaceBox *surface_box = brilliance_as_object(env, surface_box_obj);
	LeaIRegion *region = brilliance_as_object(env, region_obj);
	lea_surface_box_add(surface_box, region, index, weight);
}

/*
 * Class:     net_natpad_brilliance_leafhopper_Leafhopper
 * Method:    surfaceGetTabModel
 * Signature: (Lnet/natpad/brilliance/leafhopper/Surface;)J
 */
JNIEXPORT jlong JNICALL Java_net_natpad_brilliance_leafhopper_Leafhopper_surfaceGetTabModel
  (JNIEnv *env, jclass cls, jobject surface_obj) {
	LeaSurface *surface = brilliance_as_object(env, surface_obj);
	LeaSurfaceTabModel *result = lea_surface_get_tab_model(surface);
	return (jlong) cat_ref_ptr(result);
}



/*
 * Class:     net_natpad_brilliance_leafhopper_Leafhopper
 * Method:    surfaceTabModelAddTab
 * Signature: (Lnet/natpad/brilliance/leafhopper/SurfaceTabModel;Lnet/natpad/brilliance/leafhopper/SurfaceTab;)V
 */
JNIEXPORT void JNICALL Java_net_natpad_brilliance_leafhopper_Leafhopper_surfaceTabModelAddTab
  (JNIEnv *env, jclass cls, jobject surface_tab_model_obj, jobject surface_tab_obj)
{
	LeaSurfaceTabModel *surface_tab_model = brilliance_as_object(env, surface_tab_model_obj);
	LeaSurfaceTab *surface_tab = brilliance_as_object(env, surface_tab_obj);
	lea_surface_tab_model_add_tab(surface_tab_model, surface_tab);
}

/*
 * Class:     net_natpad_brilliance_leafhopper_Leafhopper
 * Method:    createSurfaceTab
 * Signature: (Lnet/natpad/brilliance/gtk/GtkWidget;Ljava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_net_natpad_brilliance_leafhopper_Leafhopper_createSurfaceTab
  (JNIEnv *env, jclass cls, jobject content_object, jstring label) {
	GtkWidget *content = brilliance_as_object(env, content_object);
	const char *chrs = (*env)->GetStringUTFChars(env, label, NULL);
	CatStringWo *tlabel = cat_string_wo_new_with(chrs);
	tlabel = cat_string_wo_anchor(tlabel, 0);
	(*env)->ReleaseStringUTFChars(env, label, chrs);
	jlong result = (jlong) lea_surface_tab_new(GTK_WIDGET(content), tlabel);
	cat_unref_ptr(tlabel);
	return result;
}

/*
 * Class:     net_natpad_brilliance_leafhopper_Leafhopper
 * Method:    createSurfaceBox
 * Signature: (Z)J
 */
JNIEXPORT jlong JNICALL Java_net_natpad_brilliance_leafhopper_Leafhopper_createSurfaceBox
  (JNIEnv *env, jclass cls, jboolean horizontal) {
	return (jlong) lea_surface_box_new(horizontal ? LEA_SB_HORIZONTAL : LEA_SB_VERTICAL);
}

/*
 * Class:     net_natpad_brilliance_leafhopper_Leafhopper
 * Method:    createFrame
 * Signature: (Lnet/natpad/brilliance/gtk/GtkWindow;)J
 */
JNIEXPORT jlong JNICALL Java_net_natpad_brilliance_leafhopper_Leafhopper_createFrame
  (JNIEnv *env, jclass cls, jobject window_obj) {
	GtkWindow *window = brilliance_as_object(env, window_obj);
	return (jlong) lea_frame_new(window);
}
