/*
   File:    brichacursorwo.h
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

#ifndef BRICHACURSORWO_H_
#define BRICHACURSORWO_H_

#include <jni.h>
#include <caterpillar.h>
#include <chameleon.h>

G_BEGIN_DECLS

jobject bri_cha_line_location_as_java(JNIEnv *env, ChaLineLocationWo *line_location);

ChaLineLocationWo *bri_cha_line_location_from_java(JNIEnv *env, jobject line_location_obj);

jobject bri_cha_cursor_wo_as_java(JNIEnv *env, ChaCursorWo *cursor);

ChaCursorWo *bri_cha_cursor_wo_from_java(JNIEnv *env, jobject cursor_obj);

G_END_DECLS


#endif /* BRICHACURSORWO_H_ */
