/*
   File:    bridraacentry.h
   Project: brilliance-lib-jni
   Author:  Douwe Vos
   Date:    Dec 4, 2019
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

#ifndef BRIDRAACENTRY_H_
#define BRIDRAACENTRY_H_

#include <dragonfly.h>
#include <caterpillar.h>
#include <jni.h>

G_BEGIN_DECLS

#define BRI_TYPE_DRA_AC_ENTRY              (bri_dra_ac_entry_get_type())
#define BRI_DRA_AC_ENTRY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), bri_dra_ac_entry_get_type(), BriDraAcEntry))
#define BRI_DRA_AC_ENTRY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), BRI_TYPE_DRA_AC_ENTRY, BriDraAcEntryClass))
#define BRI_IS_DRA_AC_ENTRY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), BRI_TYPE_DRA_AC_ENTRY))
#define BRI_IS_DRA_AC_ENTRY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), BRI_TYPE_DRA_AC_ENTRY))
#define BRI_DRA_AC_ENTRY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), BRI_TYPE_DRA_AC_ENTRY, BriDraAcEntryClass))

struct _BriDraAcEntry {
	DraAcEntry parent;
};

struct _BriDraAcEntryClass {
	DraAcEntryClass parent_class;
};

typedef struct _BriDraAcEntry               BriDraAcEntry;
typedef struct _BriDraAcEntryPrivate        BriDraAcEntryPrivate;
typedef struct _BriDraAcEntryClass          BriDraAcEntryClass;


GType bri_dra_ac_entry_get_type();

BriDraAcEntry *bri_dra_ac_entry_new(JNIEnv *env, CatStringWo *text);
BriDraAcEntry *bri_dra_ac_entry_new_full(JNIEnv *env, CatStringWo *entry_text, CatStringWo *type_text, gboolean enabled, int color_code, int type_color_code);

void bri_dra_ac_entry_link(BriDraAcEntry *entry, JNIEnv *env, jobject java_link);
void bri_dra_ac_entry_unlink(BriDraAcEntry *entry, JNIEnv *env);


G_END_DECLS

#endif /* BRIDRAACENTRY_H_ */
