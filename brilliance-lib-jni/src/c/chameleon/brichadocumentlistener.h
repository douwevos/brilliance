/*
   File:    brichadocumentlistener.h
   Project: brilliance-lib-jni
   Author:  Douwe Vos
   Date:    Sep 10, 2019
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

#ifndef BRICHADOCUMENTLISTENER_H_
#define BRICHADOCUMENTLISTENER_H_

#include <jni.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define BRI_CHA_TYPE_DOCUMENT_LISTENER              (bri_cha_document_listener_get_type())
#define BRI_CHA_DOCUMENT_LISTENER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), bri_cha_document_listener_get_type(), BriChaDocumentListener))
#define BRI_CHA_DOCUMENT_LISTENER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), BRI_CHA_TYPE_DOCUMENT_LISTENER, BriChaDocumentListenerClass))
#define BRI_CHA_IS_DOCUMENT_LISTENER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), BRI_CHA_TYPE_DOCUMENT_LISTENER))
#define BRI_CHA_IS_DOCUMENT_LISTENER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), BRI_CHA_TYPE_DOCUMENT_LISTENER))
#define BRI_CHA_DOCUMENT_LISTENER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), BRI_CHA_TYPE_DOCUMENT_LISTENER, BriChaDocumentListenerClass))

struct _BriChaDocumentListener {
	GObject parent;
};

struct _BriChaDocumentListenerClass {
	GObjectClass parent_class;
};

typedef struct _BriChaDocumentListener               BriChaDocumentListener;
typedef struct _BriChaDocumentListenerPrivate        BriChaDocumentListenerPrivate;
typedef struct _BriChaDocumentListenerClass          BriChaDocumentListenerClass;


GType bri_cha_document_listener_get_type();

BriChaDocumentListener *bri_cha_document_listener_new(JNIEnv *env, jobject hook);

G_END_DECLS

#endif /* BRICHADOCUMENTLISTENER_H_ */
