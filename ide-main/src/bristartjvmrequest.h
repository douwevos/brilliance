/*
   File:    bristartjvmrequest.h
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

#ifndef BRISTARTJVMREQUEST_H_
#define BRISTARTJVMREQUEST_H_

#include <caterpillar.h>
#include <worm.h>
#include <leafhopper.h>
#include <chameleon.h>

G_BEGIN_DECLS

#define BRI_TYPE_START_JVM_REQUEST              (bri_start_jvm_request_get_type())
#define BRI_START_JVM_REQUEST(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), bri_start_jvm_request_get_type(), BriStartJvmRequest))
#define BRI_START_JVM_REQUEST_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), BRI_TYPE_START_JVM_REQUEST, BriStartJvmRequestClass))
#define BRI_IS_START_JVM_REQUEST(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), BRI_TYPE_START_JVM_REQUEST))
#define BRI_IS_START_JVM_REQUEST_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), BRI_TYPE_START_JVM_REQUEST))
#define BRI_START_JVM_REQUEST_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), BRI_TYPE_START_JVM_REQUEST, BriStartJvmRequestClass))

struct _BriStartJvmRequest {
	WorRequest parent;
};

struct _BriStartJvmRequestClass {
	WorRequestClass parent_class;
};

typedef struct _BriStartJvmRequest               BriStartJvmRequest;
typedef struct _BriStartJvmRequestPrivate        BriStartJvmRequestPrivate;
typedef struct _BriStartJvmRequestClass          BriStartJvmRequestClass;


GType bri_start_jvm_request_get_type();

BriStartJvmRequest *bri_start_jvm_request_new(LeaFrame *frame, ChaDocumentManager *document_manager, WorService *wor_service);

void bri_start_jvm_request_run(BriStartJvmRequest *request);

G_END_DECLS


#endif /* BRISTARTJVMREQUEST_H_ */
