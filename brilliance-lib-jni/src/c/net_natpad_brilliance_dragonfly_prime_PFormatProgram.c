/*
   File:    net_natpad_brilliance_dragonfly_prime_PFormatProgram.c
   Project: brilliance-lib-jni
   Author:  Douwe Vos
   Date:    Dec 30, 2019
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


#include "net_natpad_brilliance_dragonfly_prime_PFormatProgram.h"

#include "brilliance_native_helper.h"
#include <jni.h>
#include <caterpillar.h>
#include <dragonfly.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "PFormatProgram"
#include <logging/catlog.h>


/*
 * Class:     net_natpad_brilliance_dragonfly_prime_PFormatProgram
 * Method:    create
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_net_natpad_brilliance_dragonfly_prime_PFormatProgram_create
  (JNIEnv *env, jclass cls) {
	DraFormatProgram *result = dra_format_program_new();
	return (jlong) result;
}

/*
 * Class:     net_natpad_brilliance_dragonfly_prime_PFormatProgram
 * Method:    construct
 * Signature: (Lnet/natpad/brilliance/dragonfly/FormatProgram;)V
 */
JNIEXPORT void JNICALL Java_net_natpad_brilliance_dragonfly_prime_PFormatProgram_construct
  (JNIEnv *env, jclass cls, jobject format_program_obj) {

}

/*
 * Class:     net_natpad_brilliance_dragonfly_prime_PFormatProgram
 * Method:    add
 * Signature: (Lnet/natpad/brilliance/dragonfly/FormatProgram;Lnet/natpad/brilliance/dragonfly/FormatAction;)V
 */
JNIEXPORT void JNICALL Java_net_natpad_brilliance_dragonfly_prime_PFormatProgram_add
  (JNIEnv *env, jclass cls, jobject format_program_obj, jobject format_action_obj) {
	DraFormatProgram *format_program = brilliance_as_object(env, format_program_obj);
	DraFormatAction *format_action = brilliance_as_object(env, format_action_obj);
	dra_format_program_add(format_program, format_action);
}

/*
 * Class:     net_natpad_brilliance_dragonfly_prime_PFormatProgram
 * Method:    apply
 * Signature: (Lnet/natpad/brilliance/dragonfly/FormatProgram;Lnet/natpad/brilliance/chameleon/Document;)V
 */
JNIEXPORT void JNICALL Java_net_natpad_brilliance_dragonfly_prime_PFormatProgram_apply
  (JNIEnv *env, jclass cls, jobject format_program_obj, jobject document_obj) {
	DraFormatProgram *format_program = brilliance_as_object(env, format_program_obj);
	ChaDocument *document = brilliance_as_object(env, document_obj);
	ChaRevisionWo *e_revision = cha_document_get_editable_revision(document);
	dra_format_program_apply(format_program, e_revision);
	cha_document_anchor_document(document);
}

/*
 * Class:     net_natpad_brilliance_dragonfly_prime_PFormatProgram
 * Method:    createAction
 * Signature: (IIIIIIII)J
 */
JNIEXPORT jlong JNICALL Java_net_natpad_brilliance_dragonfly_prime_PFormatProgram_createAction
  (JNIEnv *env, jclass cls, jint in_left_offset, jint in_left_row, jint in_right_offset, jint in_right_row,
		  jint out_left_offset, jint out_left_row, jint out_right_offset, jint out_right_row) {
	DraFormatLocation in_left = {in_left_row, in_left_offset};
	DraFormatLocation in_right = {in_right_row, in_right_offset};
	DraFormatLocation out_left = {out_left_row, out_left_offset};
	DraFormatLocation out_right = {out_right_row, out_right_offset};
	return (jlong) dra_format_action_new(in_left, in_right, out_left, out_right);
}
