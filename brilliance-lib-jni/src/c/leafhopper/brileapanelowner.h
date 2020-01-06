/*
   File:    brileapanelowner.h
   Project: brilliance-lib-jni
   Author:  Douwe Vos
   Date:    Nov 17, 2019
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

#ifndef BRILEAPANELOWNER_H_
#define BRILEAPANELOWNER_H_

#include <jni.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define BRI_LEA_TYPE_PANEL_OWNER              (bri_lea_panel_owner_get_type())
#define BRI_LEA_PANEL_OWNER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), bri_lea_panel_owner_get_type(), BriLeaPanelOwner))
#define BRI_LEA_PANEL_OWNER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), BRI_LEA_TYPE_PANEL_OWNER, BriLeaPanelOwnerClass))
#define BRI_LEA_IS_PANEL_OWNER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), BRI_LEA_TYPE_PANEL_OWNER))
#define BRI_LEA_IS_PANEL_OWNER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), BRI_LEA_TYPE_PANEL_OWNER))
#define BRI_LEA_PANEL_OWNER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), BRI_LEA_TYPE_PANEL_OWNER, BriLeaPanelOwnerClass))

struct _BriLeaPanelOwner {
	GObject parent;
};

struct _BriLeaPanelOwnerClass {
	GObjectClass parent_class;
};

typedef struct _BriLeaPanelOwner               BriLeaPanelOwner;
typedef struct _BriLeaPanelOwnerPrivate        BriLeaPanelOwnerPrivate;
typedef struct _BriLeaPanelOwnerClass          BriLeaPanelOwnerClass;


GType bri_lea_panel_owner_get_type();

BriLeaPanelOwner *bri_lea_panel_owner_new(JNIEnv *env, jobject panel_owner_java);

G_END_DECLS

#endif /* BRILEAPANELOWNER_H_ */
