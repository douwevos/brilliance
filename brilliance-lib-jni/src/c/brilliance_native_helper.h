/*
   File:    brilliance_native_helper.h
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

#ifndef BRILLIANCE_NATIVE_HELPER_H_
#define BRILLIANCE_NATIVE_HELPER_H_

#include <caterpillar.h>
#include <jni.h>

jlong brilliance_get_id(JNIEnv *env, jobject instance);
void *brilliance_as_object(JNIEnv *env, jobject obj);

JNIEnv* brilliance_attach_java(JavaVM *jvm);
void brilliance_detach_java(JavaVM *jvm);

jstring brilliance_as_java_string(JNIEnv *env, CatStringWo *text);
CatStringWo *brilliance_from_java_string(JNIEnv *env, jstring jtext);


#endif /* BRILLIANCE_NATIVE_HELPER_H_ */
