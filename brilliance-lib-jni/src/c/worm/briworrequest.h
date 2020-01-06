/*
   File:    briworrequest.h
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

#ifndef BRIWORREQUEST_H_
#define BRIWORREQUEST_H_

#include <jni.h>
#include <worm.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define BRI_WOR_TYPE_REQUEST              (bri_wor_request_get_type())
#define BRI_WOR_REQUEST(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), bri_wor_request_get_type(), BriWorRequest))
#define BRI_WOR_REQUEST_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), BRI_WOR_TYPE_REQUEST, BriWorRequestClass))
#define BRI_WOR_IS_REQUEST(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), BRI_WOR_TYPE_REQUEST))
#define BRI_WOR_IS_REQUEST_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), BRI_WOR_TYPE_REQUEST))
#define BRI_WOR_REQUEST_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), BRI_WOR_TYPE_REQUEST, BriWorRequestClass))

struct _BriWorRequest {
	WorRequest parent;
};

struct _BriWorRequestClass {
	WorRequestClass parent_class;
};

typedef struct _BriWorRequest               BriWorRequest;
typedef struct _BriWorRequestPrivate        BriWorRequestPrivate;
typedef struct _BriWorRequestClass          BriWorRequestClass;


GType bri_wor_request_get_type();

BriWorRequest *bri_wor_request_new(JNIEnv *env);

void bri_wor_request_link(BriWorRequest *request, jobject java_link);

G_END_DECLS

#endif /* BRIWORREQUEST_H_ */
