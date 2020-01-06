/*
   File:    brilliancewindow.c
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

#include "brilliancewindow.h"
#include "bristartjvmrequest.h"
#include <worm.h>
#include <leafhopper.h>
#include <chameleon.h>
#include <jni.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_ALL
#define CAT_LOG_CLAZZ "BrillianceWindow"
#include <logging/catlog.h>

struct _BrillianceWindowPrivate {
	WorService *wor_service;
	LeaFrame *frame;
};

G_DEFINE_TYPE_WITH_CODE(BrillianceWindow, brilliance_window, GTK_TYPE_APPLICATION_WINDOW,  // @suppress("Unused static function")
	G_ADD_PRIVATE(BrillianceWindow)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void brilliance_window_class_init(BrillianceWindowClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void brilliance_window_init(BrillianceWindow *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	BrillianceWindow *instance = BRILLIANCE_WINDOW(object);
//	BrillianceWindowPrivate *priv = brilliance_window_get_instance_private(instance);
	G_OBJECT_CLASS(brilliance_window_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(brilliance_window_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

void brilliance_jvm_start();

BrillianceWindow *brilliance_window_new(GApplication *application) {
	BrillianceWindow *result = g_object_new(BRILLIANCE_TYPE_WINDOW, "application", application, NULL);

	cat_ref_anounce(result);
	BrillianceWindowPrivate *priv = brilliance_window_get_instance_private(result);
//	GTK_APPLICATION_WINDOW_construct((GtkApplicationWindow *) result);
	priv->wor_service = wor_service_new();

	gtk_widget_set_size_request((GtkWidget *) result, 1040, 350);

	gtk_window_set_title(GTK_WINDOW(result), "Brilliance");

	LeaFrame *frame = lea_frame_new((GtkWindow *) result);
	priv->frame = frame;
	gtk_container_add(GTK_CONTAINER(result), GTK_WIDGET(frame));

	ChaDocumentManager *document_manager = cha_document_manager_new();

	BriStartJvmRequest *req = bri_start_jvm_request_new(frame, document_manager, priv->wor_service);
//	wor_service_post_request(priv->wor_service, req);
	bri_start_jvm_request_run(req);
	return result;
}

//
//void brilliance_jvm_start() {
//
//	  JavaVM *vm;
//	  JNIEnv *env;
//	  JavaVMInitArgs vm_args;
//	  JavaVMOption options[1];
//	  options[0].optionString = "-Djava.class.path=/home/dvos/cpp-workspace/natpad/xantus-java/dung/aa0c4a432ec684ab/xantus-java.jar";
////	  options[0].optionString = "-Djava.class.path=/home/dvos/cpp-workspace/brilliance/brilliance-lib-java/dung/b977ce62f98158b2/brilliance-lib.jar";
////	  options[1].optionString = "-Djava.library.path=/usr/lib/jvm/oracle-java8-jdk-amd64/jre/lib/amd64/:/usr/lib/jvm/oracle-java8-jdk-amd64/jre/lib/amd64/server/:/home/dvos/cpp-workspace/natpad/natpad-lib/dung/b977ce62f98158b2/:/home/dvos/cpp-workspace/brilliance/brilliance-lib-jni/dung/b977ce62f98158b2/";
//	  vm_args.version = JNI_VERSION_1_2;
//	  vm_args.options = options;
//	  vm_args.nOptions = 1;
//	  vm_args.ignoreUnrecognized = 1;
//	  jstring jstr;
//	  jint res = JNI_CreateJavaVM(&vm, (void **)&env, &vm_args);
//	  cat_log_error("jvm:res=%d", res)
//	//  if (res < 0) {
//	//    printf("Can't create Java VM\n");
//	//    exit(1);
//	//    }
//	  jclass cls = (*env)->FindClass(env, "net.natpad.brilliance.BrillianceLink"); // in c:/myclasses
//	  cat_log_error("jvm:cls=%p", cls)
//	//  if (cls == 0) {
//	//    printf("HelloWorld class not found\n");
//	//    exit(1);
//	//    }
//	  jmethodID mid =
//			  (*env)->GetStaticMethodID(env, cls, "main", "([Ljava/lang/String;)V");
//	  cat_log_error("jvm:mid=%p", mid)
//	//  if (mid == 0) {
//	//    printf("main() method not found\n");
//	//    exit(1);
//	//    }
//	  jstring argString = (*env)->NewStringUTF(env, ""); //empty arg list
//	  cat_log_error("jvm:argString=%p", argString)
//	  jobjectArray args =
//			  (*env)->NewObjectArray(env, 1, (*env)->FindClass(env, "java/lang/String"), jstr);
//	//  if (args == 0) {
//	//    printf("Out of memory\n");
//	//    exit(1);
//	//    }
//	  cat_log_error("jvm:args=%p", args)
//	  (*env)->CallStaticVoidMethod(env, cls, mid, args);
//	  sleep(20);
//	//  return 0;
//
//}
