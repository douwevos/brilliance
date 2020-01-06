/*
   File:    bridraconnectorrequestfactory.h
   Project: brilliance-lib-jni
   Author:  Douwe Vos
   Date:    Sep 15, 2019
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

#ifndef BRIDRACONNECTORREQUESTFACTORY_H_
#define BRIDRACONNECTORREQUESTFACTORY_H_

#include <jni.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define BRI_DRA_TYPE_CONNECTOR_REQUEST_FACTORY              (bri_dra_connector_request_factory_get_type())
#define BRI_DRA_CONNECTOR_REQUEST_FACTORY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), bri_dra_connector_request_factory_get_type(), BriDraConnectorRequestFactory))
#define BRI_DRA_CONNECTOR_REQUEST_FACTORY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), BRI_DRA_TYPE_CONNECTOR_REQUEST_FACTORY, BriDraConnectorRequestFactoryClass))
#define BRI_DRA_IS_CONNECTOR_REQUEST_FACTORY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), BRI_DRA_TYPE_CONNECTOR_REQUEST_FACTORY))
#define BRI_DRA_IS_CONNECTOR_REQUEST_FACTORY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), BRI_DRA_TYPE_CONNECTOR_REQUEST_FACTORY))
#define BRI_DRA_CONNECTOR_REQUEST_FACTORY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), BRI_DRA_TYPE_CONNECTOR_REQUEST_FACTORY, BriDraConnectorRequestFactoryClass))

struct _BriDraConnectorRequestFactory {
	GObject parent;
};

struct _BriDraConnectorRequestFactoryClass {
	GObjectClass parent_class;
};

typedef struct _BriDraConnectorRequestFactory               BriDraConnectorRequestFactory;
typedef struct _BriDraConnectorRequestFactoryPrivate        BriDraConnectorRequestFactoryPrivate;
typedef struct _BriDraConnectorRequestFactoryClass          BriDraConnectorRequestFactoryClass;


GType bri_dra_connector_request_factory_get_type();

BriDraConnectorRequestFactory *bri_dra_connector_request_factory_new(JNIEnv *env, jobject factory_java);

jobject bri_dra_connector_request_factory_get_link_ref(BriDraConnectorRequestFactory *factory, JNIEnv *env);

G_END_DECLS

#endif /* BRIDRACONNECTORREQUESTFACTORY_H_ */
