/*
   File:    brillianceapp.h
   Project: ide-main
   Author:  Douwe Vos
   Date:    Sep 7, 2019
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

#ifndef BRILLIANCEAPP_H_
#define BRILLIANCEAPP_H_

#include <gtk/gtk.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define BRILLIANCE_TYPE_APP              (brilliance_app_get_type())
#define BRILLIANCE_APP(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), brilliance_app_get_type(), BrillianceApp))
#define BRILLIANCE_APP_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), BRILLIANCE_TYPE_APP, BrillianceAppClass))
#define BRILLIANCE_IS_APP(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), BRILLIANCE_TYPE_APP))
#define BRILLIANCE_IS_APP_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), BRILLIANCE_TYPE_APP))
#define BRILLIANCE_APP_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), BRILLIANCE_TYPE_APP, BrillianceAppClass))

struct _BrillianceApp {
	GtkApplication parent;
};

struct _BrillianceAppClass {
	GtkApplicationClass parent_class;
};

typedef struct _BrillianceApp               BrillianceApp;
typedef struct _BrillianceAppPrivate        BrillianceAppPrivate;
typedef struct _BrillianceAppClass          BrillianceAppClass;


GType brilliance_app_get_type();

BrillianceApp *brilliance_app_new();

G_END_DECLS



#endif /* BRILLIANCEAPP_H_ */
