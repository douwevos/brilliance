/*
   File:    net_natpad_brilliance_dragonfly_prime_PDragonflyEditor.c
   Project: brilliance-lib-jni
   Author:  Douwe Vos
   Date:    Dec 27, 2019
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

#include "net_natpad_brilliance_dragonfly_prime_PDragonflyEditor.h"
#include "brilliance_native_helper.h"
#include <jni.h>
#include <caterpillar.h>
#include <dragonfly.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "PDragonflyEditor"
#include <logging/catlog.h>

/*
 * Class:     net_natpad_brilliance_dragonfly_prime_PDragonflyEditor
 * Method:    getDocument
 * Signature: (Lnet/natpad/brilliance/dragonfly/DragonflyEditor;)Lnet/natpad/brilliance/chameleon/Document;
 */
JNIEXPORT jobject JNICALL Java_net_natpad_brilliance_dragonfly_prime_PDragonflyEditor_getDocument
  (JNIEnv *env, jclass cls, jobject editor_obj) {
	DraEditor *dra_editor = brilliance_as_object(env, editor_obj);
	ChaDocument *document = cha_editor_get_document((ChaEditor *) dra_editor);
	jobject result = NULL;
	if (document) {
		jclass document_cls = (*env)->FindClass(env, "net/natpad/brilliance/chameleon/Document");
		jmethodID mi_document_constructor = (*env)->GetMethodID(env, document_cls, "<init>", "(J)V");
		result = (*env)->NewObject(env, document_cls, mi_document_constructor, (jlong) document);
		g_object_ref(document);
	}
	return result;
}
