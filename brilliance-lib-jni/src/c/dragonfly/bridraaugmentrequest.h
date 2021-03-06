/*
   File:    bridraaugmentrequest.h
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

#ifndef BRIDRAAUGMENTREQUEST_H_
#define BRIDRAAUGMENTREQUEST_H_

#include <jni.h>
#include <dragonfly.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define BRI_DRA_TYPE_AUGMENT_REQUEST              (bri_dra_augment_request_get_type())
#define BRI_DRA_AUGMENT_REQUEST(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), bri_dra_augment_request_get_type(), BriDraAugmentRequest))
#define BRI_DRA_AUGMENT_REQUEST_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), BRI_DRA_TYPE_AUGMENT_REQUEST, BriDraAugmentRequestClass))
#define BRI_DRA_IS_AUGMENT_REQUEST(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), BRI_DRA_TYPE_AUGMENT_REQUEST))
#define BRI_DRA_IS_AUGMENT_REQUEST_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), BRI_DRA_TYPE_AUGMENT_REQUEST))
#define BRI_DRA_AUGMENT_REQUEST_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), BRI_DRA_TYPE_AUGMENT_REQUEST, BriDraAugmentRequestClass))

struct _BriDraAugmentRequest {
	DraAugmentRequest parent;
};

struct _BriDraAugmentRequestClass {
	DraAugmentRequestClass parent_class;
};

typedef struct _BriDraAugmentRequest               BriDraAugmentRequest;
typedef struct _BriDraAugmentRequestPrivate        BriDraAugmentRequestPrivate;
typedef struct _BriDraAugmentRequestClass          BriDraAugmentRequestClass;


GType bri_dra_augment_request_get_type();

BriDraAugmentRequest *bri_dra_augment_request_new(JNIEnv *env, ChaDocument *document, ChaRevisionWo *revision, CatStringWo *slot_key);

void bri_dra_augment_request_link(BriDraAugmentRequest *request, jobject java_link);
void bri_dra_augment_request_unlink(BriDraAugmentRequest *request, JNIEnv *env);

G_END_DECLS

#endif /* BRIDRAAUGMENTREQUEST_H_ */
