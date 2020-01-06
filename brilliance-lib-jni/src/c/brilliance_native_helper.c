/*
   File:    brilliance_native_helper.c
   Project: brilliance-lib-jni
   Author:  Douwe Vos
   Date:    Sep 12, 2019
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


#include "brilliance_native_helper.h"

jlong brilliance_get_id(JNIEnv *env, jobject instance) {
	jclass instance_cls = (*env)->GetObjectClass(env, instance);
	jfieldID fieldId = (*env)->GetFieldID(env, instance_cls, "id", "J");
	if (fieldId==NULL) {
		return 0;
	}
	jlong id = (*env)->GetLongField(env, instance, fieldId);
	return id;
}

void *brilliance_as_object(JNIEnv *env, jobject obj) {
	if (obj==NULL) {
		return NULL;
	}
	jlong r = brilliance_get_id(env, obj);
	return r==0 ? NULL : (void *) r;
}


JNIEnv* brilliance_attach_java(JavaVM *jvm) {
    JavaVMAttachArgs args = {JNI_VERSION_1_2, 0, 0};
    JNIEnv* java;
    (*jvm)->AttachCurrentThread(jvm, (void**) &java, &args);
    return java;
}


void brilliance_detach_java(JavaVM *jvm) {
    (*jvm)->DetachCurrentThread(jvm);
}


jstring brilliance_as_java_string(JNIEnv *env, CatStringWo *text) {
	if (text == NULL) {
		return NULL;
	}
	return (*env)->NewStringUTF(env, cat_string_wo_getchars(text));
}


CatStringWo *brilliance_from_java_string(JNIEnv *env, jstring jtext) {
	CatStringWo *result = NULL;
	if (jtext) {
		const char* chrs = (*env)->GetStringUTFChars(env, jtext, NULL);
		int len = (*env)->GetStringUTFLength(env, jtext);
		result = cat_string_wo_new_with_len(chrs, len);
		(*env)->ReleaseStringUTFChars(env, jtext, chrs);
	}
	return result;
}
