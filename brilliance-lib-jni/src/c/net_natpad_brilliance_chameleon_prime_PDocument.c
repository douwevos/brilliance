/*
   File:    net_natpad_brilliance_chameleon_prime_PDocument.c
   Project: brilliance-lib-jni
   Author:  Douwe Vos
   Date:    Jan 2, 2020
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

#include "net_natpad_brilliance_chameleon_prime_PDocument.h"
#include "brilliance_native_helper.h"
#include <jni.h>
#include <caterpillar.h>
#include <leafhopper.h>
#include <chameleon.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "PDocument"
#include <logging/catlog.h>


/*
 * Class:     net_natpad_brilliance_chameleon_prime_PDocument
 * Method:    getEditableRevision
 * Signature: (Lnet/natpad/brilliance/chameleon/Document;)Lnet/natpad/brilliance/chameleon/Revision;
 */
JNIEXPORT jobject JNICALL Java_net_natpad_brilliance_chameleon_prime_PDocument_getEditableRevision
  (JNIEnv *env, jclass cls, jobject document_obj) {
	ChaDocument *document = brilliance_as_object(env, document_obj);
	ChaRevisionWo *revision = cha_document_get_editable_revision(document);
	jobject result = NULL;
	if (revision) {
		jclass revision_reader_cls = (*env)->FindClass(env, "net/natpad/brilliance/chameleon/Revision");
		if (revision_reader_cls == NULL) {
			cat_log_error("Class not found:Revision");
			return NULL;
		}
		jmethodID mod_d = (*env)->GetMethodID(env, revision_reader_cls, "<init>", "(J)V");
		result = (*env)->NewObject(env, revision_reader_cls, mod_d, (jlong) revision);
		cat_ref_ptr(revision);
	}
	return result;
}

/*
 * Class:     net_natpad_brilliance_chameleon_prime_PDocument
 * Method:    isEditable
 * Signature: (Lnet/natpad/brilliance/chameleon/Document;)Z
 */
JNIEXPORT jboolean JNICALL Java_net_natpad_brilliance_chameleon_prime_PDocument_isEditable
  (JNIEnv *env, jclass cls, jobject document_obj) {
	ChaDocument *document = brilliance_as_object(env, document_obj);
	gboolean is_editable = cha_document_is_editable(document);
	return is_editable ? JNI_TRUE : JNI_FALSE;
}

/*
 * Class:     net_natpad_brilliance_chameleon_prime_PDocument
 * Method:    anchorDocument
 * Signature: (Lnet/natpad/brilliance/chameleon/Document;)V
 */
JNIEXPORT void JNICALL Java_net_natpad_brilliance_chameleon_prime_PDocument_anchorDocument
(JNIEnv *env, jclass cls, jobject document_obj) {
	ChaDocument *document = brilliance_as_object(env, document_obj);
	cha_document_anchor_document(document);
}
