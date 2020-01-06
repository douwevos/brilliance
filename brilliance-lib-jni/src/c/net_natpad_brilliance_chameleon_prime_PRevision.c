/*
   File:    net_natpad_brilliance_chameleon_prime_PRevision.c
   Project: brilliance-lib-jni
   Author:  Douwe Vos
   Date:    Jan 1, 2020
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2020 Douwe Vos.

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

#ifndef NET_NATPAD_BRILLIANCE_CHAMELEON_PRIME_PREVISION_C_
#define NET_NATPAD_BRILLIANCE_CHAMELEON_PRIME_PREVISION_C_

#include "net_natpad_brilliance_chameleon_prime_PRevision.h"
#include "brilliance_native_helper.h"
#include "chameleon/brichacursorwo.h"
#include <caterpillar.h>
#include <leafhopper.h>
#include <chameleon.h>
#include <worm.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "PRevision"
#include <logging/catlog.h>

/*
 * Class:     net_natpad_brilliance_chameleon_prime_PRevision
 * Method:    toCursorM
 * Signature: (Lnet/natpad/brilliance/chameleon/Revision;Lnet/natpad/brilliance/chameleon/Cursor;)Lnet/natpad/brilliance/chameleon/CursorM;
 */
JNIEXPORT jobject JNICALL Java_net_natpad_brilliance_chameleon_prime_PRevision_toCursorM
  (JNIEnv *env, jclass cls, jobject revision_obj, jobject cursor_obj) {
	ChaRevisionWo *revision = brilliance_as_object(env, revision_obj);

	ChaCursorWo *cursor = bri_cha_cursor_wo_from_java(env, cursor_obj);
	cat_log_debug("cursor=%O", cursor);
	ChaCursorMWo *mcursor = cha_revision_wo_to_cursorm(revision, cursor);
	cat_unref_ptr(cursor);
	if (mcursor == NULL) {
		return NULL;
	}

	jclass revision_reader_cls = (*env)->FindClass(env, "net/natpad/brilliance/chameleon/CursorM");
	if (revision_reader_cls == NULL) {
		cat_log_error("Class not found:CursorM");
		return NULL;
	}
	jmethodID mod_d = (*env)->GetMethodID(env, revision_reader_cls, "<init>", "(JI)V");
	cat_log_debug("mod_d=%p", mod_d);
	long long row = cha_cursorm_wo_get_row(mcursor);
	int column = cha_cursorm_wo_get_column(mcursor);
	cat_unref_ptr(mcursor);
	return (*env)->NewObject(env, revision_reader_cls, mod_d, row, column);
}


/*
 * Class:     net_natpad_brilliance_chameleon_prime_PRevision
 * Method:    pageAt
 * Signature: (Lnet/natpad/brilliance/chameleon/Revision;I)Lnet/natpad/brilliance/chameleon/Page;
 */
JNIEXPORT jobject JNICALL Java_net_natpad_brilliance_chameleon_prime_PRevision_pageAt
  (JNIEnv *env, jclass cls, jobject revision_obj, jint page_index) {
	ChaRevisionWo *revision = brilliance_as_object(env, revision_obj);
	ChaPageWo *page = cha_revision_wo_page_at(revision, page_index);
	jobject result = NULL;
	if (page) {
		jclass page_cls = (*env)->FindClass(env, "net/natpad/brilliance/chameleon/Page");
		if (page_cls == NULL) {
			cat_log_error("Class not found:Page");
			return NULL;
		}
		jmethodID mod_d = (*env)->GetMethodID(env, page_cls, "<init>", "(J)V");
		result = (*env)->NewObject(env, page_cls, mod_d, (jlong) page);
		cat_ref_ptr(page);
	}
	return result;
}

/*
 * Class:     net_natpad_brilliance_chameleon_prime_PRevision
 * Method:    pageCount
 * Signature: (Lnet/natpad/brilliance/chameleon/Revision;)I
 */
JNIEXPORT jint JNICALL Java_net_natpad_brilliance_chameleon_prime_PRevision_pageCount
  (JNIEnv *env, jclass cls, jobject revision_obj) {
	ChaRevisionWo *revision = brilliance_as_object(env, revision_obj);
	return cha_revision_wo_page_count(revision);
}

/*
 * Class:     net_natpad_brilliance_chameleon_prime_PRevision
 * Method:    setCursor
 * Signature: (Lnet/natpad/brilliance/chameleon/Revision;Lnet/natpad/brilliance/chameleon/Cursor;)V
 */
JNIEXPORT void JNICALL Java_net_natpad_brilliance_chameleon_prime_PRevision_setCursor
  (JNIEnv *env, jclass cls, jobject revision_obj, jobject cursor_obj) {
	ChaRevisionWo *revision = brilliance_as_object(env, revision_obj);
	ChaCursorWo *cursor = bri_cha_cursor_wo_from_java(env, cursor_obj);
	cha_revision_wo_set_cursor(revision, cursor);
	cat_unref_ptr(cursor);
}

/*
 * Class:     net_natpad_brilliance_chameleon_prime_PRevision
 * Method:    getCursor
 * Signature: ()Lnet/natpad/brilliance/chameleon/Cursor;
 */
JNIEXPORT jobject JNICALL Java_net_natpad_brilliance_chameleon_prime_PRevision_getCursor
  (JNIEnv *env, jclass cls, jobject revision_obj) {
	ChaRevisionWo *revision = brilliance_as_object(env, revision_obj);
	ChaCursorWo *cursor = cha_revision_wo_get_cursor(revision);
	jobject cursor_obj = bri_cha_cursor_wo_as_java(env, cursor);
	return cursor_obj;
}

/*
 * Class:     net_natpad_brilliance_chameleon_prime_PRevision
 * Method:    editablePageAt
 * Signature: (Lnet/natpad/brilliance/chameleon/Revision;I)Lnet/natpad/brilliance/chameleon/Page;
 */
JNIEXPORT jobject JNICALL Java_net_natpad_brilliance_chameleon_prime_PRevision_editablePageAt
  (JNIEnv *env, jclass cls, jobject revision_obj, jint page_index) {
	ChaRevisionWo *revision = brilliance_as_object(env, revision_obj);
	ChaPageWo *page = cha_revision_wo_editable_page_at(revision, page_index);
	jobject result = NULL;
	if (page) {
		jclass page_cls = (*env)->FindClass(env, "net/natpad/brilliance/chameleon/Page");
		if (page_cls == NULL) {
			cat_log_error("Class not found:Page");
			return NULL;
		}
		jmethodID mod_d = (*env)->GetMethodID(env, page_cls, "<init>", "(J)V");
		result = (*env)->NewObject(env, page_cls, mod_d, (jlong) page);
		cat_ref_ptr(page);
	}
	return result;
}

/*
 * Class:     net_natpad_brilliance_chameleon_prime_PRevision
 * Method:    lineAtLocation
 * Signature: (Lnet/natpad/brilliance/chameleon/Revision;Lnet/natpad/brilliance/chameleon/LineLocation;)Lnet/natpad/brilliance/chameleon/Line;
 */
JNIEXPORT jobject JNICALL Java_net_natpad_brilliance_chameleon_prime_PRevision_lineAtLocation
  (JNIEnv *env, jclass cls, jobject revision_obj, jobject line_location_obj) {
	ChaLineLocationWo *ll = bri_cha_line_location_from_java(env, line_location_obj);
	ChaRevisionWo *revision = brilliance_as_object(env, revision_obj);
	ChaLineWo *line = cha_revision_wo_line_at_location(revision, ll);
	jobject result = NULL;
	if (line) {
		jclass line_cls = (*env)->FindClass(env, "net/natpad/brilliance/chameleon/Line");
		if (line_cls == NULL) {
			cat_log_error("Class not found:Line");
			return NULL;
		}
		jmethodID mod_d = (*env)->GetMethodID(env, line_cls, "<init>", "(J)V");
		result = (*env)->NewObject(env, line_cls, mod_d, (jlong) line);
		cat_ref_ptr(line);
	}
	return result;
}


/*
 * Class:     net_natpad_brilliance_chameleon_prime_PRevision
 * Method:    insertAsLines
 * Signature: (Lnet/natpad/brilliance/chameleon/Revision;Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_net_natpad_brilliance_chameleon_prime_PRevision_insertAsLines
  (JNIEnv *env, jclass cls, jobject revision_obj, jstring jtxt) {
	ChaRevisionWo *revision = brilliance_as_object(env, revision_obj);
	const char* chrs = (*env)->GetStringUTFChars(env, jtxt, NULL);
	int len = (*env)->GetStringUTFLength(env, jtxt);
	CatStringWo *t = cat_string_wo_new_with_len(chrs, len);
	(*env)->ReleaseStringUTFChars(env, jtxt, chrs);
	CatArrayWo *line_list_out = cha_document_create_line_list(NULL, t);
	int result = cat_array_wo_size(line_list_out);
	cha_revision_wo_insert(revision, line_list_out);
	cat_unref_ptr(t);
	return (jint) result;
}


/*
 * Class:     net_natpad_brilliance_chameleon_prime_PRevision
 * Method:    calculateRow
 * Signature: (Lnet/natpad/brilliance/chameleon/Revision;Lnet/natpad/brilliance/chameleon/LineLocation;)J
 */
JNIEXPORT jlong JNICALL Java_net_natpad_brilliance_chameleon_prime_PRevision_calculateRow
  (JNIEnv *env, jclass cls, jobject revision_obj, jobject line_location_obj) {
	ChaRevisionWo *revision = brilliance_as_object(env, revision_obj);
	ChaLineLocationWo *line_location = bri_cha_line_location_from_java(env, line_location_obj);
	return (jlong) cha_revision_wo_calculate_row(revision, line_location);
}

/*
 * Class:     net_natpad_brilliance_chameleon_prime_PRevision
 * Method:    calculateLineLocation
 * Signature: (Lnet/natpad/brilliance/chameleon/Revision;J)Lnet/natpad/brilliance/chameleon/LineLocation;
 */
JNIEXPORT jobject JNICALL Java_net_natpad_brilliance_chameleon_prime_PRevision_calculateLineLocation
  (JNIEnv *env, jclass cls, jobject revision_obj, jlong row) {
	ChaRevisionWo *revision = brilliance_as_object(env, revision_obj);
	ChaLineLocationWo *line_location = cha_revision_wo_calculate_line_location(revision, row);
	jobject result = bri_cha_line_location_as_java(env, line_location);
	cat_unref_ptr(line_location);
	return result;
}

#endif
