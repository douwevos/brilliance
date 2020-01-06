/*
   File:    net_natpad_brilliance_chameleon_prime_PUow.c
   Project: brilliance-lib-jni
   Author:  Douwe Vos
   Date:    Jan 1, 2020
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

#include "net_natpad_brilliance_chameleon_prime_PUow.h"
#include "brilliance_native_helper.h"
#include "chameleon/brichacursorwo.h"
#include "chameleon/brichauowremote.h"
#include <caterpillar.h>
#include <leafhopper.h>
#include <chameleon.h>
#include <worm.h>


/*
 * Class:     net_natpad_brilliance_chameleon_prime_PUow
 * Method:    createUowRemote
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_net_natpad_brilliance_chameleon_prime_PUow_createUowRemote
  (JNIEnv *env, jclass cls) {
	return (jlong) bri_cha_uow_remote_new();
}

/*
 * Class:     net_natpad_brilliance_chameleon_prime_PUow
 * Method:    constructUowRemote
 * Signature: (Lnet/natpad/brilliance/chameleon/uow/UowRemote;)V
 */
JNIEXPORT void JNICALL Java_net_natpad_brilliance_chameleon_prime_PUow_constructUowRemote
  (JNIEnv *env, jclass cls, jobject uow_remote_obj, jboolean is_read_only) {
	BriChaUowRemote *uow_remote = brilliance_as_object(env, uow_remote_obj);
	bri_cha_uow_remote_construct(uow_remote, env, uow_remote_obj, is_read_only);
}


/*
 * Class:     net_natpad_brilliance_chameleon_prime_PUow
 * Method:    constructUowInsertText
 * Signature: (Ljava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_net_natpad_brilliance_chameleon_prime_PUow_constructUowInsertText
  (JNIEnv *env, jclass cls, jstring jtext) {
	CatStringWo *text = brilliance_from_java_string(env, jtext);
	return (jlong) cha_uow_insert_text_new(text);
}

/*
 * Class:     net_natpad_brilliance_chameleon_prime_PUow
 * Method:    uowInsertTextSetDelta
 * Signature: (Lnet/natpad/brilliance/chameleon/uow/UowInsertText;II)V
 */
JNIEXPORT void JNICALL Java_net_natpad_brilliance_chameleon_prime_PUow_uowInsertTextSetDelta
  (JNIEnv *env, jclass cls, jobject uow_insert_obj, jint delta_row, jint delta_column) {
	ChaUowInsertText *uow = brilliance_as_object(env, uow_insert_obj);
	cha_uow_insert_text_set_cursor_delta(uow, delta_row, delta_column);
}


/*
 * Class:     net_natpad_brilliance_chameleon_prime_PUow
 * Method:    constructUowReplaceText
 * Signature: (Ljava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_net_natpad_brilliance_chameleon_prime_PUow_constructUowReplaceText
(JNIEnv *env, jclass cls, jstring jtext) {
	CatStringWo *text = brilliance_from_java_string(env, jtext);
	return (jlong) cha_uow_replace_text_new(text);
}
