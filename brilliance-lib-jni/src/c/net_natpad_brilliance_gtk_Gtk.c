/*
   File:    net_natpad_brilliance_gtk_Gtk.c
   Project: brilliance-lib-jni
   Author:  Douwe Vos
   Date:    Sep 9, 2019
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
#include "net_natpad_brilliance_gtk_Gtk.h"
#include "brilliance_native_helper.h"
#include <caterpillar.h>
#include <gtk/gtk.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "BrillianceGtk"
#include <logging/catlog.h>



static jlong extract_native_id(JNIEnv *env, jobject instance) {
	jclass instance_cls = (*env)->GetObjectClass(env, instance);
	jfieldID fieldId = (*env)->GetFieldID(env, instance_cls, "id", "J");
	jlong id = (*env)->GetLongField(env, instance, fieldId);
	return id;
}

/*
 * Class:     net_natpad_brilliance_gtk_Gtk
 * Method:    add
 * Signature: (Lnet/natpad/brilliance/gtk/GtkContainer;Lnet/natpad/brilliance/gtk/GtkWidget;)V
 */
JNIEXPORT void JNICALL Java_net_natpad_brilliance_gtk_Gtk_containerAdd(JNIEnv *env, jclass cls, jobject container, jobject child) {
	jlong container_id = extract_native_id(env, container);
	jlong child_id = extract_native_id(env, child);
	gtk_container_add(GTK_CONTAINER(container_id), GTK_WIDGET(child_id));
}

/*
 * Class:     net_natpad_brilliance_gtk_Gtk
 * Method:    destroy
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_net_natpad_brilliance_gtk_Gtk_destroy(JNIEnv *env, jclass cls, jlong native_id) {
	cat_unref((GObject *) native_id);
}

/*
 * Class:     net_natpad_brilliance_gtk_Gtk
 * Method:    widgetGetParent
 * Signature: (Lnet/natpad/brilliance/gtk/GtkWidget;)J
 */
JNIEXPORT jlong JNICALL Java_net_natpad_brilliance_gtk_Gtk_widgetGetParent(JNIEnv *env, jclass cls, jobject widget) {
	jlong widget_id = extract_native_id(env, widget);
	cat_log_debug("widget_id=%p", widget_id);
	GtkWidget *result = gtk_widget_get_parent(GTK_WIDGET(widget_id));
	cat_log_debug("result=%p", result);
	return result==NULL ? 0 : (jlong) g_object_ref(result);
}

/*
 * Class:     net_natpad_brilliance_gtk_Gtk
 * Method:    widgetShowAll
 * Signature: (Lnet/natpad/brilliance/gtk/GtkWidget;)V
 */
JNIEXPORT void JNICALL Java_net_natpad_brilliance_gtk_Gtk_widgetShowAll(JNIEnv *env, jclass cls, jobject widget) {
	jlong widget_id = extract_native_id(env, widget);
	cat_log_debug("widget_id=%p", widget_id);
	cat_log_debug("is widget=%d", GTK_IS_WIDGET(widget_id));
	gtk_widget_show_all(GTK_WIDGET(widget_id));
}

/*
 * Class:     net_natpad_brilliance_gtk_Gtk
 * Method:    createScrolledWindow
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_net_natpad_brilliance_gtk_Gtk_createScrolledWindow
  (JNIEnv *env, jclass cls) {
	return (jlong) gtk_scrolled_window_new(NULL, NULL);
}

/*
 * Class:     net_natpad_brilliance_gtk_Gtk
 * Method:    setSizeRequest
 * Signature: (Lnet/natpad/brilliance/gtk/GtkWidget;II)V
 */
JNIEXPORT void JNICALL Java_net_natpad_brilliance_gtk_Gtk_widgetSetSizeRequest
  (JNIEnv *env, jclass cls, jobject widget_obj, jint width, jint height) {
	GtkWidget *widget = brilliance_as_object(env, widget_obj);
	gtk_widget_set_size_request(widget, width, height);
}

/*
 * Class:     net_natpad_brilliance_gtk_Gtk
 * Method:    main
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_net_natpad_brilliance_gtk_Gtk_main(JNIEnv *env, jclass cls) {
	gtk_main();
	cat_log_info("gtm-main finished");
}



static gboolean l_close_window_cb(GtkWidget *widget, GdkEvent *eev, gpointer main_ptr) {
	gtk_main_quit();
	return FALSE;
}

/*
 * Class:     net_natpad_brilliance_gtk_Gtk
 * Method:    createWindow
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_net_natpad_brilliance_gtk_Gtk_createWindow(JNIEnv *env, jclass cls) {
	gtk_init(NULL, NULL);

	GtkWidget *result = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect(G_OBJECT(result), "delete-event", G_CALLBACK(l_close_window_cb), result);

	return (jlong) result;
}
