/*
   File:    brilliancewindow.h
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

#ifndef BRILLIANCEWINDOW_H_
#define BRILLIANCEWINDOW_H_

#include <caterpillar.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define BRILLIANCE_TYPE_WINDOW              (brilliance_window_get_type())
#define BRILLIANCE_WINDOW(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), brilliance_window_get_type(), BrillianceWindow))
#define BRILLIANCE_WINDOW_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), BRILLIANCE_TYPE_WINDOW, BrillianceWindowClass))
#define BRILLIANCE_IS_WINDOW(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), BRILLIANCE_TYPE_WINDOW))
#define BRILLIANCE_IS_WINDOW_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), BRILLIANCE_TYPE_WINDOW))
#define BRILLIANCE_WINDOW_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), BRILLIANCE_TYPE_WINDOW, BrillianceWindowClass))

struct _BrillianceWindow {
	GtkApplicationWindow parent;
};

struct _BrillianceWindowClass {
	GtkApplicationWindowClass parent_class;
};

typedef struct _BrillianceWindow               BrillianceWindow;
typedef struct _BrillianceWindowPrivate        BrillianceWindowPrivate;
typedef struct _BrillianceWindowClass          BrillianceWindowClass;


GType brilliance_window_get_type();

BrillianceWindow *brilliance_window_new(GApplication *application);

G_END_DECLS

#endif /* BRILLIANCEWINDOW_H_ */
