/*
   File:    brichauowremote.h
   Project: brilliance-lib-jni
   Author:  Douwe Vos
   Date:    Jan 2, 2020
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

#ifndef BRICHAUOWREMOTE_H_
#define BRICHAUOWREMOTE_H_

#include <jni.h>
#include <caterpillar.h>
#include <chameleon.h>

G_BEGIN_DECLS

#define BRI_TYPE_CHA_UOW_REMOTE              (bri_cha_uow_remote_get_type())
#define BRI_CHA_UOW_REMOTE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), bri_cha_uow_remote_get_type(), BriChaUowRemote))
#define BRI_CHA_UOW_REMOTE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), BRI_TYPE_CHA_UOW_REMOTE, BriChaUowRemoteClass))
#define BRI_IS_CHA_UOW_REMOTE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), BRI_TYPE_CHA_UOW_REMOTE))
#define BRI_IS_CHA_UOW_REMOTE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), BRI_TYPE_CHA_UOW_REMOTE))
#define BRI_CHA_UOW_REMOTE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), BRI_TYPE_CHA_UOW_REMOTE, BriChaUowRemoteClass))

struct _BriChaUowRemote {
	ChaUow parent;
};

struct _BriChaUowRemoteClass {
	ChaUowClass parent_class;
};

typedef struct _BriChaUowRemote               BriChaUowRemote;
typedef struct _BriChaUowRemotePrivate        BriChaUowRemotePrivate;
typedef struct _BriChaUowRemoteClass          BriChaUowRemoteClass;


GType bri_cha_uow_remote_get_type();

BriChaUowRemote *bri_cha_uow_remote_new();

void bri_cha_uow_remote_construct(BriChaUowRemote *uow_remote, JNIEnv *env, jobject uow_remote_obj, jboolean is_read_only);

G_END_DECLS

#endif /* BRICHAUOWREMOTE_H_ */
