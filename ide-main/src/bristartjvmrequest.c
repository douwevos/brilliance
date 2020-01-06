/*
   File:    bristartjvmrequest.c
   Project: ide-main
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

#include "bristartjvmrequest.h"
#include <jni.h>
#include <dlfcn.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_ALL
#define CAT_LOG_CLAZZ "BriStartJvmRequest"
#include <logging/catlog.h>

struct _BriStartJvmRequestPrivate {
	LeaFrame *frame;
	ChaDocumentManager *document_manager;
	WorService *wor_service;
	JavaVM *vm;
	JNIEnv *env;
};

G_DEFINE_TYPE_WITH_CODE(BriStartJvmRequest, bri_start_jvm_request, WOR_TYPE_REQUEST,  // @suppress("Unused static function")
	G_ADD_PRIVATE(BriStartJvmRequest)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_run_request(WorRequest *request);

static void bri_start_jvm_request_class_init(BriStartJvmRequestClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	WorRequestClass *wor_class = WOR_REQUEST_CLASS(clazz);
	wor_class->runRequest = l_run_request;
}

static void bri_start_jvm_request_init(BriStartJvmRequest *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	BriStartJvmRequest *instance = BRI_START_JVM_REQUEST(object);
//	BriStartJvmRequestPrivate *priv = bri_start_jvm_request_get_instance_private(instance);
	G_OBJECT_CLASS(bri_start_jvm_request_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(bri_start_jvm_request_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


BriStartJvmRequest *bri_start_jvm_request_new(LeaFrame *frame, ChaDocumentManager *document_manager, WorService *wor_service) {
	BriStartJvmRequest *result = g_object_new(BRI_TYPE_START_JVM_REQUEST, NULL);
	cat_ref_anounce(result);
	BriStartJvmRequestPrivate *priv = bri_start_jvm_request_get_instance_private(result);
	priv->frame = cat_ref_ptr(frame);
	priv->document_manager = cat_ref_ptr(document_manager);
	priv->wor_service = cat_ref_ptr(wor_service);
	wor_request_construct((WorRequest *) result);
	wor_request_set_time_out(result, cat_date_current_time()+1000);
	return result;
}



void brilliance_jvm_start(BriStartJvmRequest *request) {
	BriStartJvmRequestPrivate *priv = bri_start_jvm_request_get_instance_private(request);

	JavaVMInitArgs vm_args;
	JavaVMOption options[2];
//	  options[0].optionString = "-Djava.class.path=/home/dvos/cpp-workspace/natpad/xantus-java/dung/aa0c4a432ec684ab/xantus-java.jar";
	options[0].optionString = "-Djava.class.path=/home/dvos/cpp-workspace/brilliance/brilliance-lib-java/dung/b977ce62f98158b2/brilliance-lib.jar";
	options[1].optionString = "-Djava.library.path=/usr/lib/jvm/oracle-java8-jdk-amd64/jre/lib/amd64/:/usr/lib/jvm/oracle-java8-jdk-amd64/jre/lib/amd64/server/:/home/dvos/cpp-workspace/natpad/natpad-lib/dung/b977ce62f98158b2/:/home/dvos/cpp-workspace/brilliance/brilliance-lib-jni/dung/b977ce62f98158b2/";
	vm_args.version = JNI_VERSION_1_2;
	vm_args.options = options;
	vm_args.nOptions = 2;
	vm_args.ignoreUnrecognized = 1;
	  jstring jstr;
	  jint res = JNI_CreateJavaVM(&(priv->vm), (void **)&(priv->env), &vm_args);
	  cat_log_error("jvm:res=%d", res)
	//  if (res < 0) {
	//    printf("Can't create Java VM\n");
	//    exit(1);
	//    }
	  jclass cls = (*(priv->env))->FindClass((priv->env), "net/natpad/brilliance/BrillianceLink"); // in c:/myclasses
	  cat_log_error("jvm:cls=%p", cls)
	//  if (cls == 0) {
	//    printf("HelloWorld class not found\n");
	//    exit(1);
	//    }
	  jmethodID mid =
			  (*(priv->env))->GetStaticMethodID((priv->env), cls, "main", "([Ljava/lang/String;)V");
	  cat_log_error("jvm:mid=%p", mid)
	//  if (mid == 0) {
	//    printf("main() method not found\n");
	//    exit(1);
	//    }

	  cat_log_debug("Brilliance: Linking Frame and DocumentManager");

	  jfieldID  filedId = (*(priv->env))->GetStaticFieldID((priv->env), cls, "frameId", "J");
	  cat_log_error("filedId=%p, frame=%p", filedId, priv->frame);
	  (*(priv->env))->SetStaticLongField((priv->env), cls, filedId, (jlong) priv->frame);

	  jfieldID  filedId2 = (*(priv->env))->GetStaticFieldID((priv->env), cls, "documentManagerId", "J");
	  cat_log_error("filedId2=%p, frame=%p", filedId2, priv->document_manager);
	  (*(priv->env))->SetStaticLongField((priv->env), cls, filedId2, (jlong) priv->document_manager);

	  jfieldID  filedId3 = (*(priv->env))->GetStaticFieldID((priv->env), cls, "workerServiceId", "J");
	  cat_log_error("filedId3=%p, frame=%p", filedId2, priv->wor_service);
	  (*(priv->env))->SetStaticLongField((priv->env), cls, filedId3, (jlong) priv->wor_service);


	  /* initalizing worm service */
	  cat_log_debug("Brilliance: Linking Worm-service");
	  jclass wormCls = (*(priv->env))->FindClass((priv->env), "net/natpad/brilliance/worm/Worm"); // in c:/myclasses
	  cat_log_debug("Brilliance: wormCls=%p", wormCls);
	  jmethodID midWormService =
			  (*(priv->env))->GetStaticMethodID((priv->env), wormCls, "setupWormService", "(J)V");
	  (*(priv->env))->CallStaticVoidMethod((priv->env), wormCls, midWormService, (jlong) priv->wor_service);




	  cat_log_debug("Brilliance: Invoking main");

	  jstring argString = (*(priv->env))->NewStringUTF((priv->env), ""); //empty arg list
	  cat_log_error("jvm:argString=%p", argString)
	  jobjectArray args =
			  (*(priv->env))->NewObjectArray((priv->env), 1, (*(priv->env))->FindClass((priv->env), "java/lang/String"), jstr);
	//  if (args == 0) {
	//    printf("Out of memory\n");
	//    exit(1);
	//    }
	  cat_log_error("jvm:args=%p", args)
	  (*(priv->env))->CallStaticVoidMethod((priv->env), cls, mid, args);
	//  return 0;
	  cat_log_error("done")

}


static void l_run_request(WorRequest *request) {
	cat_log_info("did request");
	brilliance_jvm_start(BRI_START_JVM_REQUEST(request));
}


void bri_start_jvm_request_run(BriStartJvmRequest *request) {
	brilliance_jvm_start(request);
}

