/*
   File:    net_natpad_brilliance_dragonfly_Dragonfly.c
   Project: brilliance-lib-jni
   Author:  Douwe Vos
   Date:    Sep 11, 2019
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

#ifndef NET_NATPAD_BRILLIANCE_DRAGONFLY_DRAGONFLY_C_
#define NET_NATPAD_BRILLIANCE_DRAGONFLY_DRAGONFLY_C_

#include "brilliance_native_helper.h"
#include "net_natpad_brilliance_dragonfly_Dragonfly.h"
#include "dragonfly/bridraacentry.h"
#include "dragonfly/bridraaugmentrequest.h"
#include "dragonfly/bridraconnectorrequestfactory.h"
#include "dragonfly/bridrapanelowner.h"
#include "dragonfly/bridracontexteditor.h"
#include "dragonfly/bridradocumentview.h"
#include "dragonfly/bridraeditor.h"
#include "dragonfly/bridraeditorpanel.h"
#include "leafhopper/brileapanelowner.h"
#include "chameleon/brichacursorwo.h"
#include <caterpillar.h>
#include <leafhopper.h>
#include <chameleon.h>
#include <dragonfly.h>
#include <worm.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "BrillianceDragonfly"
#include <logging/catlog.h>

/*
 * Class:     net_natpad_brilliance_dragonfly_Dragonfly
 * Method:    createEditor
 * Signature: (Lnet/natpad/brilliance/chameleon/Document;Lnet/natpad/brilliance/dragonfly/ConnetorMap;Lnet/natpad/brilliance/dragonfly/IConnectorRequestFactory;Lnet/natpad/brilliance/worm/WorkerService;)J
 */
JNIEXPORT jlong JNICALL Java_net_natpad_brilliance_dragonfly_Dragonfly_createEditor
  (JNIEnv *env, jclass cls) {
	cat_log_debug("start");
//	cat_log_debug("connector_factory=%p", connector_factory);
	return (jlong) bri_dra_editor_new();
}


/*
 * Class:     net_natpad_brilliance_dragonfly_Dragonfly
 * Method:    constructEditor
 * Signature: (Lnet/natpad/brilliance/dragonfly/DragonflyEditor;Lnet/natpad/brilliance/chameleon/Document;Lnet/natpad/brilliance/dragonfly/ConnectorMap;Lnet/natpad/brilliance/dragonfly/IConnectorRequestFactory;Lnet/natpad/brilliance/worm/WorkerService;)V
 */
JNIEXPORT void JNICALL Java_net_natpad_brilliance_dragonfly_Dragonfly_constructEditor
  (JNIEnv *env, jclass cls, jobject editor_obj, jobject document_obj, jobject connector_map_obj, jobject factory_obj, jobject worm_service_obj) {

	BriDraEditor *editor = brilliance_as_object(env, editor_obj);
	ChaDocument *document = brilliance_as_object(env, document_obj);
	DraConnectorMap *connector_map = brilliance_as_object(env, connector_map_obj);
	BriDraConnectorRequestFactory *factory = bri_dra_connector_request_factory_new(env, factory_obj);
	WorService *wor_service = brilliance_as_object(env, worm_service_obj);
	bri_dra_editor_construct(editor, env, editor_obj, document, connector_map, (DraIConnectorRequestFactory *) factory, wor_service);
}


/*
 * Class:     net_natpad_brilliance_dragonfly_Dragonfly
 * Method:    createConnectorMap
 * Signature: (Lnet/natpad/brilliance/worm/WorkerService;)J
 */
JNIEXPORT jlong JNICALL Java_net_natpad_brilliance_dragonfly_Dragonfly_createConnectorMap
  (JNIEnv *env, jclass cls, jobject worm_service_obj) {
	WorService *wor_service = brilliance_as_object(env, worm_service_obj);
	return (jlong) dra_connector_map_new(wor_service);
}


/*
 * Class:     net_natpad_brilliance_dragonfly_Dragonfly
 * Method:    createKeywordPrinter
 * Signature: (Lnet/natpad/brilliance/chameleon/Revision;Lnet/natpad/brilliance/caterpillar/StringWo;I)J
 */
JNIEXPORT jlong JNICALL Java_net_natpad_brilliance_dragonfly_Dragonfly_createKeywordPrinter
  (JNIEnv *env, jclass cls, jobject revision_obj, jobject markup_key_obj, jint markup_idx) {
	ChaRevisionWo *revision = brilliance_as_object(env, revision_obj);
	CatStringWo *markup_key = brilliance_as_object(env, markup_key_obj);
	return (jlong) dra_keyword_printer_new(revision, markup_key, markup_idx);
}

/*
 * Class:     net_natpad_brilliance_dragonfly_Dragonfly
 * Method:    createAugmentRequest
 * Signature: (Lnet/natpad/brilliance/chameleon/Document;Lnet/natpad/brilliance/chameleon/Revision;Lnet/natpad/brilliance/caterpillar/StringWo;)J
 */
JNIEXPORT jlong JNICALL Java_net_natpad_brilliance_dragonfly_Dragonfly_createAugmentRequest
  (JNIEnv *env, jclass cls, jobject document_obj, jobject revision_obj, jobject markup_key_obj) {
	ChaDocument *document = brilliance_as_object(env, document_obj);
	ChaRevisionWo *revision = brilliance_as_object(env, revision_obj);
	CatStringWo *markup_key = brilliance_as_object(env, markup_key_obj);
	return (jlong) bri_dra_augment_request_new(env, document, revision, markup_key);
}

/*
 * Class:     net_natpad_brilliance_dragonfly_Dragonfly
 * Method:    augmentRequestLink
 * Signature: (Lnet/natpad/brilliance/dragonfly/AugmentRequest;)V
 */
JNIEXPORT void JNICALL Java_net_natpad_brilliance_dragonfly_Dragonfly_augmentRequestLink
  (JNIEnv *env, jclass cls, jobject augment_request_obj) {
	BriDraAugmentRequest *augment_request = brilliance_as_object(env, augment_request_obj);
	cat_log_debug("NewGlobalRef:1");
	jobject java_link = (*env)->NewGlobalRef(env, augment_request_obj);
	cat_log_debug("NewGlobalRef:2");
	bri_dra_augment_request_link(augment_request, java_link);
	cat_log_debug("NewGlobalRef:3: %d", ((GObject *) augment_request)->ref_count);
}

/*
 * Class:     net_natpad_brilliance_dragonfly_Dragonfly
 * Method:    augmentRequestLink
 * Signature: (Lnet/natpad/brilliance/dragonfly/AugmentRequest;)V
 */
JNIEXPORT void JNICALL Java_net_natpad_brilliance_dragonfly_Dragonfly_augmentRequestUnlink
  (JNIEnv *env, jclass cls, jobject augment_request_obj) {
	BriDraAugmentRequest *augment_request = brilliance_as_object(env, augment_request_obj);
	bri_dra_augment_request_unlink(augment_request, env);
	cat_log_debug("DeleteGlobalRef:3: %d", ((GObject *) augment_request)->ref_count);
}

/*
 * Class:     net_natpad_brilliance_dragonfly_Dragonfly
 * Method:    keyWordPrinterPrintFgColor
 * Signature: (Lnet/natpad/brilliance/dragonfly/KeywordPrinter;IIIII)V
 */
JNIEXPORT void JNICALL Java_net_natpad_brilliance_dragonfly_Dragonfly_keyWordPrinterPrintFgColor
  (JNIEnv *env, jclass cls, jobject key_wordprinter_obj, jint begin_row, jint begin_column, jint end_row, jint end_column, jint color_index) {
	DraKeywordPrinter *keyword_printer = brilliance_as_object(env, key_wordprinter_obj);
	dra_keyword_printer_print_fg_color(keyword_printer, begin_row, begin_column, end_row, end_column, color_index);

}

/*
 * Class:     net_natpad_brilliance_dragonfly_Dragonfly
 * Method:    createEditorPanel
 * Signature: (Lnet/natpad/brilliance/leafhopper/IPanelOwner;Lnet/natpad/brilliance/chameleon/Document;Lnet/natpad/brilliance/dragonfly/IConnectorRequestFactory;)J
 */
/*
 * Class:     net_natpad_brilliance_dragonfly_Dragonfly
 * Method:    createEditorPanel
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_net_natpad_brilliance_dragonfly_Dragonfly_createEditorPanel
  (JNIEnv *env, jclass cls) {
	return (jlong) bri_dra_editor_panel_new();
}


/*
 * Class:     net_natpad_brilliance_dragonfly_Dragonfly
 * Method:    createPanelOwner
 * Signature: (Lnet/natpad/brilliance/leafhopper/Frame;Lnet/natpad/brilliance/worm/WorkerService;)J
 */
JNIEXPORT jlong JNICALL Java_net_natpad_brilliance_dragonfly_Dragonfly_createPanelOwner
  (JNIEnv *env, jclass cls, jobject frame_obj, jobject worm_service_obj) {
	LeaFrame *frame = brilliance_as_object(env, frame_obj);
	WorService *wor_service = brilliance_as_object(env, worm_service_obj);
	return (jlong) bri_dra_panel_owner_new(env, frame, wor_service);
}

/*
 * Class:     net_natpad_brilliance_dragonfly_Dragonfly
 * Method:    editorPanelGetEditor
 * Signature: (Lnet/natpad/brilliance/dragonfly/EditorPanel;)J
 */
JNIEXPORT jlong JNICALL Java_net_natpad_brilliance_dragonfly_Dragonfly_editorPanelGetEditor
  (JNIEnv *env, jclass cls, jobject panel) {
	DraEditorPanel *editor_panel = brilliance_as_object(env, panel);
	DraEditor *result = dra_editor_panel_get_editor(editor_panel);
	cat_ref_ptr(result);
	return (jlong) result;
}

/*
 * Class:     net_natpad_brilliance_dragonfly_Dragonfly
 * Method:    createContextEditor
 * Signature: (Lnet/natpad/brilliance/dragonfly/ContextEditor;Lnet/natpad/brilliance/dragonfly/EditorPanel;)J
 */
JNIEXPORT jlong JNICALL Java_net_natpad_brilliance_dragonfly_Dragonfly_createContextEditor
  (JNIEnv *env, jclass cls, jobject context_editor_obj, jobject editor_panel_obj) {
	DraEditorPanel *editor_panel = brilliance_as_object(env, editor_panel_obj);

	cat_log_debug("create context editor");
	BriDraContextEditor *result = bri_dra_context_editor_new(env, context_editor_obj, editor_panel);
//	bri_dra_context_editor_link(result, context_editor_obj);
	cat_log_debug("create context editor done:%O", result);
	return (jlong) result;
}

/*
 * Class:     net_natpad_brilliance_dragonfly_Dragonfly
 * Method:    editorSetContextEditor
 * Signature: (Lnet/natpad/brilliance/dragonfly/DragonflyEditor;Lnet/natpad/brilliance/dragonfly/ContextEditor;)V
 */
JNIEXPORT void JNICALL Java_net_natpad_brilliance_dragonfly_Dragonfly_editorSetContextEditor
  (JNIEnv *env, jclass cls, jobject editor_obj, jobject context_editor_obj) {
	cat_log_debug("setinng context-editor");
	DraEditor *editor = brilliance_as_object(env, editor_obj);
	DraContextEditor *context_editor = brilliance_as_object(env, context_editor_obj);
	cat_log_debug("setinng context-editor:%p, context_editor=%O", editor, context_editor);
	dra_editor_set_context_editor(editor, context_editor);
}

/*
 * Class:     net_natpad_brilliance_dragonfly_Dragonfly
 * Method:    acContextCreateRevisionReader
 * Signature: (Lnet/natpad/brilliance/dragonfly/AcContext;)Lnet/natpad/brilliance/chameleon/RevisionReader;
 */
JNIEXPORT jobject JNICALL Java_net_natpad_brilliance_dragonfly_Dragonfly_acContextCreateRevisionReader
  (JNIEnv *env, jclass cls, jobject ac_context_obj) {
	DraAcContext *ac_context = brilliance_as_object(env, ac_context_obj);
	ChaRevisionReader *reader = dra_ac_context_create_revision_reader(ac_context);
	if (reader == NULL) {
		return NULL;
	}
	jclass revision_reader_cls = (*env)->FindClass(env, "net/natpad/brilliance/chameleon/RevisionReader");
	if (revision_reader_cls == NULL) {
		cat_log_error("Class not found:RevisionReader");
		return NULL;
	}
	jmethodID mod_d = (*env)->GetMethodID(env, revision_reader_cls, "<init>", "(J)V");
	cat_log_debug("create_req:6:mod_d=%p", mod_d);
	jobject result = (*env)->NewObject(env, revision_reader_cls, mod_d, (jlong) reader);
	return result;
}



/*
 * Class:     net_natpad_brilliance_dragonfly_Dragonfly
 * Method:    acContextGetEndCursor
 * Signature: (Lnet/natpad/brilliance/dragonfly/AcContext;)Lnet/natpad/brilliance/chameleon/Cursor;
 */
JNIEXPORT jobject JNICALL Java_net_natpad_brilliance_dragonfly_Dragonfly_acContextGetEndCursor
  (JNIEnv *env, jclass cls, jobject ac_context_obj) {
	DraAcContext *ac_context = brilliance_as_object(env, ac_context_obj);
	ChaCursorWo *cursor = dra_ac_context_get_end_cursor(ac_context);
	cat_log_debug("cursor=%O", cursor);
	return bri_cha_cursor_wo_as_java(env, cursor);
}

/*
 * Class:     net_natpad_brilliance_dragonfly_Dragonfly
 * Method:    acContextGetStartCursor
 * Signature: (Lnet/natpad/brilliance/dragonfly/AcContext;)Lnet/natpad/brilliance/chameleon/Cursor;
 */
JNIEXPORT jobject JNICALL Java_net_natpad_brilliance_dragonfly_Dragonfly_acContextGetStartCursor
  (JNIEnv *env, jclass cls, jobject ac_context_obj) {
	DraAcContext *ac_context = brilliance_as_object(env, ac_context_obj);
	ChaCursorWo *cursor = dra_ac_context_get_start_cursor(ac_context);
	cat_log_debug("cursor=%O", cursor);
	return bri_cha_cursor_wo_as_java(env, cursor);
}


/*
 * Class:     net_natpad_brilliance_dragonfly_Dragonfly
 * Method:    acContextGetFilterCursor
 * Signature: (Lnet/natpad/brilliance/dragonfly/AcContext;)Lnet/natpad/brilliance/chameleon/Cursor;
 */
JNIEXPORT jobject JNICALL Java_net_natpad_brilliance_dragonfly_Dragonfly_acContextGetFilterCursor
  (JNIEnv *env, jclass cls, jobject ac_context_obj) {
	DraAcContext *ac_context = brilliance_as_object(env, ac_context_obj);
	ChaCursorWo *cursor = dra_ac_context_get_filter_cursor(ac_context);
	cat_log_debug("cursor=%O", cursor);
	return bri_cha_cursor_wo_as_java(env, cursor);
}


/*
 * Class:     net_natpad_brilliance_dragonfly_Dragonfly
 * Method:    revisionToCursorM
 * Signature: (Lnet/natpad/brilliance/chameleon/Revision;Lnet/natpad/brilliance/chameleon/Cursor;)Lnet/natpad/brilliance/chameleon/CursorM;
 */
JNIEXPORT jobject JNICALL Java_net_natpad_brilliance_dragonfly_Dragonfly_revisionToCursorM
  (JNIEnv *env, jclass cls, jobject revision_obj, jobject cursor_obj) {
	ChaRevisionWo *revision = brilliance_as_object(env, revision_obj);

	ChaCursorWo *cursor = bri_cha_cursor_wo_from_java(env, cursor_obj);
	cat_log_debug("cursor=%O", cursor);
	ChaCursorMWo *mcursor = cha_revision_wo_to_cursorm(revision, cursor);
	cat_unref_ptr(cursor);
	if (mcursor == NULL) {
		return NULL;
	}

	jclass revision_reader_cls = (*env)->FindClass(env, "net/natpad/brilliance/chameleon/CursorM");
	if (revision_reader_cls == NULL) {
		cat_log_error("Class not found:CursorM");
		return NULL;
	}
	jmethodID mod_d = (*env)->GetMethodID(env, revision_reader_cls, "<init>", "(JI)V");
	cat_log_debug("create_req:6:mod_d=%p", mod_d);
	long long row = cha_cursorm_wo_get_row(mcursor);
	int column = cha_cursorm_wo_get_column(mcursor);
	cat_unref_ptr(mcursor);
	jobject result = (*env)->NewObject(env, revision_reader_cls, mod_d, row, column);
	return result;
}


/*
 * Class:     net_natpad_brilliance_dragonfly_Dragonfly
 * Method:    acContextGetRevision
 * Signature: (Lnet/natpad/brilliance/dragonfly/AcContext;)Lnet/natpad/brilliance/chameleon/Revision;
 */
JNIEXPORT jobject JNICALL Java_net_natpad_brilliance_dragonfly_Dragonfly_acContextGetRevision
  (JNIEnv *env, jclass cls, jobject ac_context_obj) {
	DraAcContext *ac_context = brilliance_as_object(env, ac_context_obj);
	ChaRevisionWo *revision = dra_ac_context_get_revision(ac_context);
	if (revision == NULL) {
		return NULL;
	}

	jclass revision_reader_cls = (*env)->FindClass(env, "net/natpad/brilliance/chameleon/Revision");
	if (revision_reader_cls == NULL) {
		cat_log_error("Class not found:Revision");
		return NULL;
	}
	jmethodID mod_d = (*env)->GetMethodID(env, revision_reader_cls, "<init>", "(J)V");
	cat_ref_ptr(revision);
	jobject result = (*env)->NewObject(env, revision_reader_cls, mod_d, (jlong) revision);
	return result;
}

/*
 * Class:     net_natpad_brilliance_dragonfly_Dragonfly
 * Method:    createLineTag
 * Signature: (JLnet/natpad/brilliance/dragonfly/LineTagType;)J
 */
JNIEXPORT jlong JNICALL Java_net_natpad_brilliance_dragonfly_Dragonfly_createLineTag
  (JNIEnv *env, jclass cls, jlong row, jobject line_tag_type_obj, jobject line_tag_level_obj) {
	jclass line_tag_type_cls = (*env)->GetObjectClass(env, line_tag_type_obj);
	jmethodID mid_line_tag_type = (*env)->GetMethodID(env, line_tag_type_cls, "ordinal", "()I");
	jint ord = (*env)->CallIntMethod(env, line_tag_type_obj, mid_line_tag_type);
	if ((*env)->ExceptionCheck(env)) {
		cat_log_error("in create line-tag");
	}

	DraTagType tp = DRA_TAG_TYPE_MODEL;
	switch(ord) {
		case 0: tp = DRA_TAG_TYPE_MODEL; break;
		case 1: tp = DRA_TAG_TYPE_PARSER; break;
		case 2: tp = DRA_TAG_TYPE_SCANNER; break;
		case 3: tp = DRA_TAG_TYPE_SPELL; break;
	}

	jclass line_tag_level_cls = (*env)->GetObjectClass(env, line_tag_level_obj);
	jmethodID mid_line_tag_level = (*env)->GetMethodID(env, line_tag_level_cls, "ordinal", "()I");
	jint ord_level = (*env)->CallIntMethod(env, line_tag_level_obj, mid_line_tag_level);
	if ((*env)->ExceptionCheck(env)) {
		cat_log_error("in create line-tag");
	}

	DraTagLevel lv = DRA_TAG_LEVEL_ERROR;
	switch(ord_level) {
		case 0: lv = DRA_TAG_LEVEL_ERROR; break;
		case 1: lv = DRA_TAG_LEVEL_WARN; break;
		case 2: lv = DRA_TAG_LEVEL_UNSAFE; break;
		case 3: lv = DRA_TAG_LEVEL_INFO; break;
		case 4: lv = DRA_TAG_LEVEL_MARK; break;

	}
	return (jlong) dra_line_tag_wo_new(row, tp, lv);
}

/*
 * Class:     net_natpad_brilliance_dragonfly_Dragonfly
 * Method:    keyWordPrinterPrintLineTag
 * Signature: (Lnet/natpad/brilliance/dragonfly/KeywordPrinter;Lnet/natpad/brilliance/dragonfly/LineTag;)V
 */
JNIEXPORT void JNICALL Java_net_natpad_brilliance_dragonfly_Dragonfly_keyWordPrinterPrintLineTag
  (JNIEnv *env, jclass cls, jobject printer_obj, jobject line_tag_obj) {
	DraKeywordPrinter *keyword_printer = brilliance_as_object(env, printer_obj);
	DraLineTagWo *tag = brilliance_as_object(env, line_tag_obj);
	cat_ref_ptr(tag);
	dra_keyword_printer_print_line_tag(keyword_printer, tag);
}

/*
 * Class:     net_natpad_brilliance_dragonfly_Dragonfly
 * Method:    lineTagSetText
 * Signature: (Lnet/natpad/brilliance/dragonfly/LineTag;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_net_natpad_brilliance_dragonfly_Dragonfly_lineTagSetText
  (JNIEnv *env, jclass cls, jobject line_tag_obj, jstring text_str) {
	const char* chrs = (*env)->GetStringUTFChars(env, text_str, NULL);
	int len = (*env)->GetStringUTFLength(env, text_str);
	CatStringWo *t = cat_string_wo_new_with_len(chrs, len);
	DraLineTagWo *tag = brilliance_as_object(env, line_tag_obj);
	dra_line_tag_wo_set_text(tag, t);
	cat_unref_ptr(t);
	(*env)->ReleaseStringUTFChars(env, text_str, chrs);
}

/*
 * Class:     net_natpad_brilliance_dragonfly_Dragonfly
 * Method:    lineTagSetStartAndEndIndex
 * Signature: (Lnet/natpad/brilliance/dragonfly/LineTag;II)V
 */
JNIEXPORT void JNICALL Java_net_natpad_brilliance_dragonfly_Dragonfly_lineTagSetStartAndEndIndex
  (JNIEnv *env, jclass cls, jobject line_tag_obj, jint start, jint end) {
	DraLineTagWo *tag = brilliance_as_object(env, line_tag_obj);
	dra_line_tag_wo_set_start_and_end_index(tag, start, end);
}

/*
 * Class:     net_natpad_brilliance_dragonfly_Dragonfly
 * Method:    lineTagSetColor
 * Signature: (Lnet/natpad/brilliance/dragonfly/LineTag;DDD)V
 */
JNIEXPORT void JNICALL Java_net_natpad_brilliance_dragonfly_Dragonfly_lineTagSetColor
  (JNIEnv *env, jclass cls, jobject line_tag_obj, jdouble jred, jdouble jgreen, jdouble jblue) {
	DraLineTagWo *tag = brilliance_as_object(env, line_tag_obj);
	dra_line_tag_wo_set_color(tag, jred, jgreen, jblue);
}


/*
 * Class:     net_natpad_brilliance_dragonfly_Dragonfly
 * Method:    createAcEntry
 * Signature: (Ljava/lang/String;)J
 */
JNIEXPORT jlong JNICALL Java_net_natpad_brilliance_dragonfly_Dragonfly_createAcEntry
  (JNIEnv *env, jclass cls, jstring text_str) {
	CatStringWo *entry_text = brilliance_from_java_string(env, text_str);
	BriDraAcEntry *entry = bri_dra_ac_entry_new(env, entry_text);
	cat_unref_ptr(entry_text);
	return (jlong) entry;
}

/*
 * Class:     net_natpad_brilliance_dragonfly_Dragonfly
 * Method:    acContextAddEntry
 * Signature: (Lnet/natpad/brilliance/dragonfly/AcContext;Lnet/natpad/brilliance/dragonfly/AcEntry;)V
 */
JNIEXPORT void JNICALL Java_net_natpad_brilliance_dragonfly_Dragonfly_acContextAddEntry
  (JNIEnv *env, jclass cls, jobject context_obj, jobject entry_obj) {
	DraAcContext *context = brilliance_as_object(env, context_obj);
	DraAcEntry *entry = brilliance_as_object(env, entry_obj);
	dra_ac_add_entry(context, entry);
}

/*
 * Class:     net_natpad_brilliance_dragonfly_Dragonfly
 * Method:    acEntryLink
 * Signature: (Lnet/natpad/brilliance/dragonfly/AcEntry;)V
 */
JNIEXPORT void JNICALL Java_net_natpad_brilliance_dragonfly_Dragonfly_acEntryLink
  (JNIEnv *env, jclass cls, jobject ac_entry_obj) {
	BriDraAcEntry *entry = brilliance_as_object(env, ac_entry_obj);

	bri_dra_ac_entry_link(entry, env, ac_entry_obj);
}

/*
 * Class:     net_natpad_brilliance_dragonfly_Dragonfly
 * Method:    acEntryUnlink
 * Signature: (Lnet/natpad/brilliance/dragonfly/AcEntry;)V
 */
JNIEXPORT void JNICALL Java_net_natpad_brilliance_dragonfly_Dragonfly_acEntryUnlink
(JNIEnv *env, jclass cls, jobject ac_entry_obj) {
	BriDraAcEntry *entry = brilliance_as_object(env, ac_entry_obj);
	bri_dra_ac_entry_unlink(entry, env);
}

/*
 * Class:     net_natpad_brilliance_dragonfly_Dragonfly
 * Method:    acContextGetEditor
 * Signature: (Lnet/natpad/brilliance/dragonfly/AcContext;)Lnet/natpad/brilliance/chameleon/ChameleonEditor;
 */
JNIEXPORT jobject JNICALL Java_net_natpad_brilliance_dragonfly_Dragonfly_acContextGetEditor
  (JNIEnv *env, jclass cls, jobject ac_context_obj) {
	DraAcContext *ac_context = brilliance_as_object(env, ac_context_obj);
	cat_log_debug("ac_context=%O", ac_context);
	ChaEditor *dra_editor = dra_ac_context_get_editor(ac_context);
	cat_log_debug("dra_editor=%O", dra_editor);
	if (BRI_IS_DRA_EDITOR(dra_editor)) {
		BriDraEditor *bri_dra_editor = BRI_DRA_EDITOR(dra_editor);
		return bri_dra_editor_get_link(bri_dra_editor, env);
	}
	return NULL;
}

/*
 * Class:     net_natpad_brilliance_dragonfly_Dragonfly
 * Method:    editorLink
 * Signature: (Lnet/natpad/brilliance/dragonfly/DragonflyEditor;)V
 */
JNIEXPORT void JNICALL Java_net_natpad_brilliance_dragonfly_Dragonfly_editorLink
  (JNIEnv *env, jclass cls, jobject editor_obj) {
	DraEditor *dra_editor = brilliance_as_object(env, editor_obj);
	if (BRI_IS_DRA_EDITOR(dra_editor)) {
		BriDraEditor *bri_dra_editor = BRI_DRA_EDITOR(dra_editor);
		bri_dra_editor_link(bri_dra_editor, env, editor_obj);
	}
}

/*
 * Class:     net_natpad_brilliance_dragonfly_Dragonfly
 * Method:    editorPanelLink
 * Signature: (Lnet/natpad/brilliance/dragonfly/EditorPanel;)V
 */
JNIEXPORT void JNICALL Java_net_natpad_brilliance_dragonfly_Dragonfly_editorPanelLink
  (JNIEnv *env, jclass cls, jobject editor_panel_obj) {
	DraEditorPanel *dra_editor_panel = brilliance_as_object(env, editor_panel_obj);
	cat_log_debug("linking dra_editor_panel=%O", dra_editor_panel);
	if (BRI_IS_DRA_EDITOR_PANEL(dra_editor_panel)) {
		BriDraEditorPanel *bri_dra_editor_panel = BRI_DRA_EDITOR_PANEL(dra_editor_panel);
		bri_dra_editor_panel_link(bri_dra_editor_panel, env, editor_panel_obj);
	}
}


/*
 * Class:     net_natpad_brilliance_dragonfly_Dragonfly
 * Method:    constructEditorPanelLink
 * Signature: (Lnet/natpad/brilliance/dragonfly/EditorPanel;Lnet/natpad/brilliance/leafhopper/IPanelOwner;Lnet/natpad/brilliance/chameleon/Document;Lnet/natpad/brilliance/dragonfly/IConnectorRequestFactory;)V
 */
JNIEXPORT void JNICALL Java_net_natpad_brilliance_dragonfly_Dragonfly_constructEditorPanelLink
  (JNIEnv *env, jclass cls, jobject editor_panel_obj, jobject panel_owner_obj, jobject document_obj, jobject factory_obj) {
	BriDraEditorPanel *editor_panel = brilliance_as_object(env, editor_panel_obj);
	ChaDocument *document = brilliance_as_object(env, document_obj);
	BriDraConnectorRequestFactory *factory = bri_dra_connector_request_factory_new(env, factory_obj);

	LeaIPanelOwner *panel_owner = brilliance_as_object(env, panel_owner_obj);
	if (panel_owner == NULL) {
		panel_owner = (LeaIPanelOwner *) bri_lea_panel_owner_new(env, panel_owner_obj);
	}
	bri_dra_editor_panel_construct(editor_panel, env, editor_panel_obj, panel_owner, document, factory);

}


/*
 * Class:     net_natpad_brilliance_dragonfly_Dragonfly
 * Method:    editorGetLineInfoKey
 * Signature: (Lnet/natpad/brilliance/dragonfly/DragonflyEditor;)J
 */
JNIEXPORT jlong JNICALL Java_net_natpad_brilliance_dragonfly_Dragonfly_editorGetLineInfoKey
  (JNIEnv *env, jclass cls, jobject editor_obj) {
	DraEditor *dra_editor = brilliance_as_object(env, editor_obj);
	return (jlong) dra_editor_get_line_info_key(dra_editor);
}

/*
 * Class:     net_natpad_brilliance_dragonfly_Dragonfly
 * Method:    createAcEntry
 * Signature: (Ljava/lang/String;)J
 */
JNIEXPORT void JNICALL Java_net_natpad_brilliance_dragonfly_Dragonfly_editorUowInsertTemplateRaw
  (JNIEnv *env, jclass cls, jobject editor_obj, jstring text_str) {
	DraEditor *dra_editor = brilliance_as_object(env, editor_obj);
	const char* chrs = (*env)->GetStringUTFChars(env, text_str, NULL);
	int len = (*env)->GetStringUTFLength(env, text_str);
	CatStringWo *t = cat_string_wo_new_with_len(chrs, len);
	ChaUow *uow = cha_uow_insert_template_new_raw(t);
	cha_editor_run((ChaEditor *) dra_editor, uow);
	cat_unref_ptr(uow);
	cat_unref_ptr(t);
	(*env)->ReleaseStringUTFChars(env, text_str, chrs);
}


/*
 * Class:     net_natpad_brilliance_dragonfly_Dragonfly
 * Method:    createDocumentView
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_net_natpad_brilliance_dragonfly_Dragonfly_createDocumentView
  (JNIEnv *env, jclass cls) {
	return (jlong) bri_dra_document_view_new();
}

/*
 * Class:     net_natpad_brilliance_dragonfly_Dragonfly
 * Method:    constructDocumentView
 * Signature: (Lnet/natpad/brilliance/dragonfly/DocumentView;Lnet/natpad/brilliance/chameleon/Document;JLnet/natpad/brilliance/gtk/GtkWidget;J)V
 */
JNIEXPORT void JNICALL Java_net_natpad_brilliance_dragonfly_Dragonfly_constructDocumentView
  (JNIEnv *env, jclass cls, jobject document_view_obj, jobject document_obj, jlong pango_context_obj, jobject widget_obj, jlong info_key_obj) {

	ChaDocumentView *document_view = brilliance_as_object(env, document_view_obj);
	ChaDocument *document = brilliance_as_object(env, document_obj);
	PangoContext *pango_context = (PangoContext *) pango_context_obj;
	GtkWidget *widget = brilliance_as_object(env, widget_obj);
	CatStringWo *info_key = (CatStringWo *) info_key_obj;

	if (BRI_IS_DRA_DOCUMENT_VIEW(document_view)) {
		bri_dra_document_view_construct((BriDraDocumentView *) document_view, env, document_view_obj, document, pango_context, widget, info_key);
	}
}

/*
 * Class:     net_natpad_brilliance_dragonfly_Dragonfly
 * Method:    constructAcEntryFull
 * Signature: (Ljava/lang/String;Ljava/lang/String;IIZ)J
 */
JNIEXPORT jlong JNICALL Java_net_natpad_brilliance_dragonfly_Dragonfly_constructAcEntryFull
  (JNIEnv *env, jclass cls, jstring jentry_text, jstring jtype_text, jint color_code, jint type_color_code, jboolean enabled) {
	CatStringWo *entry_text = brilliance_from_java_string(env, jentry_text);
	CatStringWo *type_text = brilliance_from_java_string(env, jtype_text);
	BriDraAcEntry *entry = bri_dra_ac_entry_new_full(env, entry_text, type_text, enabled==JNI_TRUE, color_code, type_color_code);
	cat_unref_ptr(entry_text);
	cat_unref_ptr(type_text);
	return (jlong) entry;
}


#endif /* NET_NATPAD_BRILLIANCE_DRAGONFLY_DRAGONFLY_C_ */
