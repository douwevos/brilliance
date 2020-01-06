/*
   File:    bridrapanelowner.h
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

#ifndef BRIDRAPANELOWNER_H_
#define BRIDRAPANELOWNER_H_

#include <jni.h>
#include <dragonfly.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define BRI_DRA_TYPE_PANEL_OWNER              (bri_dra_panel_owner_get_type())
#define BRI_DRA_PANEL_OWNER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), bri_dra_panel_owner_get_type(), BriDraPanelOwner))
#define BRI_DRA_PANEL_OWNER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), BRI_DRA_TYPE_PANEL_OWNER, BriDraPanelOwnerClass))
#define BRI_DRA_IS_PANEL_OWNER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), BRI_DRA_TYPE_PANEL_OWNER))
#define BRI_DRA_IS_PANEL_OWNER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), BRI_DRA_TYPE_PANEL_OWNER))
#define BRI_DRA_PANEL_OWNER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), BRI_DRA_TYPE_PANEL_OWNER, BriDraPanelOwnerClass))

struct _BriDraPanelOwner {
	DraPanelOwner parent;
};

struct _BriDraPanelOwnerClass {
	DraPanelOwnerClass parent_class;
};

typedef struct _BriDraPanelOwner               BriDraPanelOwner;
typedef struct _BriDraPanelOwnerPrivate        BriDraPanelOwnerPrivate;
typedef struct _BriDraPanelOwnerClass          BriDraPanelOwnerClass;


GType bri_dra_panel_owner_get_type();

BriDraPanelOwner *bri_dra_panel_owner_new(JNIEnv *env, LeaFrame *frame, WorService *service);

void bri_dra_panel_owner_link(BriDraPanelOwner *request, jobject java_link);

G_END_DECLS

#endif /* BRIDRAPANELOWNER_H_ */
