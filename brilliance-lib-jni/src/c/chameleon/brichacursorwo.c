/*
   File:    brichacursorwo.c
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

#ifndef BRICHACURSORWO_C_
#define BRICHACURSORWO_C_

#include "brichacursorwo.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "BriChaCursorWo"
#include <logging/catlog.h>

jobject bri_cha_line_location_as_java(JNIEnv *env, ChaLineLocationWo *line_location) {
	if (line_location == NULL) {
		return NULL;
	}

	jclass line_location_cls = (*env)->FindClass(env, "net/natpad/brilliance/chameleon/LineLocation");
	if (line_location_cls == NULL) {
		cat_log_error("Class not found:LineLocation");
		return NULL;
	}
	jmethodID mod_d = (*env)->GetMethodID(env, line_location_cls, "<init>", "(II)V");
	int page_index = cha_line_location_wo_get_page_index(line_location);
	int page_line_index = cha_line_location_wo_get_page_line_index(line_location);
	jobject result = (*env)->NewObject(env, line_location_cls, mod_d, page_index, page_line_index);
	return result;
}

jobject bri_cha_cursor_wo_as_java(JNIEnv *env, ChaCursorWo *cursor) {
	if (cursor == NULL) {
		return NULL;
	}

	ChaLineLocationWo *line_location = cha_cursor_wo_get_line_location(cursor);
	jobject line_location_obj = bri_cha_line_location_as_java(env, line_location);
	jclass cursor_cls = (*env)->FindClass(env, "net/natpad/brilliance/chameleon/Cursor");

	jmethodID mod_d = (*env)->GetMethodID(env, cursor_cls, "<init>", "(Lnet/natpad/brilliance/chameleon/LineLocation;II)V");
	int x_cursor_bytes = cha_cursor_wo_get_x_cursor_bytes(cursor);
	int x_sub = cha_cursor_wo_get_x_sub(cursor);
	jobject result = (*env)->NewObject(env, cursor_cls, mod_d, line_location_obj, x_cursor_bytes, x_sub);
	cat_log_debug("result=%p", result);
	return result;

}

ChaLineLocationWo *bri_cha_line_location_from_java(JNIEnv *env, jobject line_location_obj) {
	jclass cursor_class = (*env)->GetObjectClass(env, line_location_obj);
	jmethodID mod_d = (*env)->GetMethodID(env, cursor_class, "getPageIndex", "()I");
	jint page_index = (*env)->CallIntMethod(env, line_location_obj, mod_d);
	if ((*env)->ExceptionCheck(env)) {
		cat_log_error("in here");
		return NULL;
	}

	mod_d = (*env)->GetMethodID(env, cursor_class, "getPageLineIndex", "()I");
	jint page_line_index = (*env)->CallIntMethod(env, line_location_obj, mod_d);
	if ((*env)->ExceptionCheck(env)) {
		cat_log_error("in here");
		return NULL;
	}
	return cha_line_location_wo_new_with(page_index, page_line_index);
}

ChaCursorWo *bri_cha_cursor_wo_from_java(JNIEnv *env, jobject cursor_obj) {
	if (cursor_obj == NULL) {
		return NULL;
	}
	jclass cursor_class = (*env)->GetObjectClass(env, cursor_obj);

	jmethodID mod_d = (*env)->GetMethodID(env, cursor_class, "getLineLocation", "()Lnet/natpad/brilliance/chameleon/LineLocation;");
	jobject line_location_obj = (*env)->CallObjectMethod(env, cursor_obj, mod_d);
	if ((*env)->ExceptionCheck(env)) {
		cat_log_error("in here");
		return NULL;
	}
	ChaLineLocationWo *ll = NULL;
	if (line_location_obj) {
		ll = bri_cha_line_location_from_java(env, line_location_obj);
	}

	mod_d = (*env)->GetMethodID(env, cursor_class, "getByteOffset", "()I");
	jint byte_offset = (*env)->CallIntMethod(env, cursor_obj, mod_d);
	if ((*env)->ExceptionCheck(env)) {
		cat_log_error("in here");
		return NULL;
	}
	mod_d = (*env)->GetMethodID(env, cursor_class, "getSubOffset", "()I");
	jint sub_offset = (*env)->CallIntMethod(env, cursor_obj, mod_d);
	if ((*env)->ExceptionCheck(env)) {
		cat_log_error("in here");
		return NULL;
	}

	ChaCursorWo *result = cha_cursor_wo_new_ll_offset(ll, byte_offset);
	cha_cursor_wo_set_x_sub(result, sub_offset);
	cat_unref_ptr(ll);
	return result;
}


#endif /* BRICHACURSORWO_C_ */
