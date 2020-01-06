/*
   File:    net_natpad_brilliance_worm_Worm.c
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
#include "net_natpad_brilliance_worm_Worm.h"
#include "worm/briworrequest.h"
#include <worm.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "BrillianceWorm"
#include <logging/catlog.h>


/*
 * Class:     net_natpad_brilliance_worm_Worm
 * Method:    createRequest
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_net_natpad_brilliance_worm_Worm_createRequest
  (JNIEnv *env, jclass cls) {
	return (jlong) bri_wor_request_new(env);
}

/*
 * Class:     net_natpad_brilliance_worm_Worm
 * Method:    requestLink
 * Signature: (Lnet/natpad/brilliance/worm/Request;)V
 */
JNIEXPORT void JNICALL Java_net_natpad_brilliance_worm_Worm_requestLink
  (JNIEnv *env, jclass cls, jobject request_obj) {
	BriWorRequest *request = brilliance_as_object(env, request_obj);
	jobject java_link = (*env)->NewGlobalRef(env, request_obj);
	bri_wor_request_link(request, java_link);
}

/*
 * Class:     net_natpad_brilliance_worm_Worm
 * Method:    workerServicePostRequest
 * Signature: (Lnet/natpad/brilliance/worm/WorkerService;Lnet/natpad/brilliance/worm/Request;)V
 */
JNIEXPORT void JNICALL Java_net_natpad_brilliance_worm_Worm_workerServicePostRequest
  (JNIEnv *env, jclass cls, jobject worker_service_obj, jobject request_obj) {
	WorService *wor_service = brilliance_as_object(env, worker_service_obj);
	WorRequest *request = brilliance_as_object(env, request_obj);
	wor_service_post_request(wor_service, request);
}


/*
 * Class:     net_natpad_brilliance_worm_Worm
 * Method:    createWorkerService
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_net_natpad_brilliance_worm_Worm_createWorkerService(JNIEnv *env, jclass cls) {
	return (jlong) wor_service_new();
}
