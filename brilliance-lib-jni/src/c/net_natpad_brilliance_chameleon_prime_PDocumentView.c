/*
   File:    net_natpad_brilliance_chameleon_prime_PDocumentView.c
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

#include "net_natpad_brilliance_chameleon_prime_PDocumentView.h"
#include "brilliance_native_helper.h"
#include <jni.h>
#include <caterpillar.h>
#include <chameleon.h>


/*
 * Class:     net_natpad_brilliance_chameleon_prime_PDocumentView
 * Method:    removeSelection
 * Signature: (Lnet/natpad/brilliance/chameleon/DocumentView;)Z
 */
JNIEXPORT jboolean JNICALL Java_net_natpad_brilliance_chameleon_prime_PDocumentView_removeSelection
  (JNIEnv *env, jclass cls, jobject document_view_obj) {
	ChaDocumentView *document_view = brilliance_as_object(env, document_view_obj);
	gboolean result = cha_document_view_remove_selection(document_view);
	return result==TRUE ? JNI_TRUE : JNI_FALSE;
}

/*
 * Class:     net_natpad_brilliance_chameleon_prime_PDocumentView
 * Method:    moveViewToFocus
 * Signature: (Lnet/natpad/brilliance/chameleon/DocumentView;Z)V
 */
JNIEXPORT void JNICALL Java_net_natpad_brilliance_chameleon_prime_PDocumentView_moveViewToFocus
  (JNIEnv *env, jclass cls, jobject document_view_obj, jboolean do_center) {
	ChaDocumentView *document_view = brilliance_as_object(env, document_view_obj);
	cha_document_view_move_view_to_focus(document_view, do_center==JNI_TRUE);
}
