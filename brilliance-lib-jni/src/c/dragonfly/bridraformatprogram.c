///*
//   File:    bridraformatprogram.c
//   Project: brilliance-lib-jni
//   Author:  Douwe Vos
//   Date:    Dec 30, 2019
//   e-mail:  dmvos2000(at)yahoo.com
//
//   Copyright (C) 2019 Douwe Vos.
//
//   This program is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 3 of the License, or
//   (at your option) any later version.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */
//
//#include "BriDraFormatProgram.h"
//
//#include <logging/catlogdefs.h>
//#define CAT_LOG_LEVEL CAT_LOG_WARN
//#define CAT_LOG_CLAZZ "BriDraFormatProgram"
//#include <logging/catlog.h>
//
//struct _BriDraFormatProgramPrivate {
//	void *dummy;
//};
//
//G_DEFINE_TYPE_WITH_CODE(BriDraFormatProgram, bri_dra_format_program, DRA_TYPE_FORMAT_PROGRAM,  // @suppress("Unused static function")
//	G_ADD_PRIVATE(BriDraFormatProgram)
//);
//
//static void l_dispose(GObject *object);
//static void l_finalize(GObject *object);
//
//static void bri_dra_format_program_class_init(BriDraFormatProgramClass *clazz) {
//	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
//	object_class->dispose = l_dispose;
//	object_class->finalize = l_finalize;
//}
//
//static void bri_dra_format_program_init(BriDraFormatProgram *instance) {
//}
//
//static void l_dispose(GObject *object) {
//	cat_log_detail("dispose:%p", object);
////	BriDraFormatProgram *instance = BRI_DRA_FORMAT_PROGRAM(object);
////	BriDraFormatProgramPrivate *priv = bri_dra_format_program_get_instance_private(instance);
//	G_OBJECT_CLASS(bri_dra_format_program_parent_class)->dispose(object);
//	cat_log_detail("disposed:%p", object);
//}
//
//static void l_finalize(GObject *object) {
//	cat_log_detail("finalize:%p", object);
//	cat_ref_denounce(object);
//	G_OBJECT_CLASS(bri_dra_format_program_parent_class)->finalize(object);
//	cat_log_detail("finalized:%p", object);
//}
//
//
//BriDraFormatProgram *bri_dra_format_program_new() {
//	BriDraFormatProgram *result = g_object_new(BRI_TYPE_DRA_FORMAT_PROGRAM, NULL);
//	cat_ref_anounce(result);
//	BriDraFormatProgramPrivate *priv = bri_dra_format_program_get_instance_private(result);
////	DRA_FORMAT_PROGRAM_construct((DraFormatProgram *) result);
//	return result;
//}
//
//
//
//
//
//
//
//
//
