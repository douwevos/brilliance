/*
   File:    net_natpad_brilliance_caterpillar_StringWo.c
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
#include "net_natpad_brilliance_caterpillar_StringWo.h"
#include <caterpillar.h>


JNIEXPORT jlong JNICALL Java_net_natpad_brilliance_caterpillar_StringWo_createString(JNIEnv *env, jobject instance) {
	return (jlong) cat_string_wo_new();
}

/*
 * Class:     net_natpad_brilliance_caterpillar_StringWo
 * Method:    destroyString
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_net_natpad_brilliance_caterpillar_StringWo_destroyString(JNIEnv *env, jobject instance, jlong id) {
	CatStringWo *obj = (CatStringWo *) id;
	cat_unref_ptr(obj);
}





