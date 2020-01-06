/*
   File:    net_natpad_brilliance_chameleon_Chameleon.c
   Project: brilliance-lib-jni
   Author:  Douwe Vos
   Date:    Sep 10, 2019
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

#include "chameleon/brichadocumentlistener.h"
#include "chameleon/brichadocumentview.h"
#include "chameleon/brichacursorwo.h"
#include "dragonfly/bridradocumentview.h"
#include "brilliance_native_helper.h"
#include "net_natpad_brilliance_chameleon_Chameleon.h"
#include <jni.h>
#include <caterpillar.h>
#include <leafhopper.h>
#include <chameleon.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "BrillianceChameleon"
#include <logging/catlog.h>

/*
 * Class:     net_natpad_brilliance_chameleon_Chameleon
 * Method:    createEditor
 * Signature: (Lnet/natpad/brilliance/chameleon/Document;)J
 */
JNIEXPORT jlong JNICALL Java_net_natpad_brilliance_chameleon_Chameleon_createEditor
  (JNIEnv *env, jclass cls, jobject document_obj) {
	ChaDocument *document = brilliance_as_object(env, document_obj);
	cat_log_debug("document=%p", document);
	return (jlong) cha_editor_new(document);
}

/*
 * Class:     net_natpad_brilliance_chameleon_Chameleon
 * Method:    createDocument
 * Signature: (Lnet/natpad/brilliance/chameleon/DocumentManager;)J
 */
JNIEXPORT jlong JNICALL Java_net_natpad_brilliance_chameleon_Chameleon_createDocument
  (JNIEnv *env, jclass cls, jobject document_manager_obj) {
	ChaDocumentManager *document_manager = brilliance_as_object(env, document_manager_obj);
	cat_log_debug("document_manager=%p", document_manager);
	return (jlong) cha_document_new(document_manager);
}

/*
 * Class:     net_natpad_brilliance_chameleon_Chameleon
 * Method:    documentHookListener
 * Signature: (JLnet/natpad/brilliance/chameleon/DocumentListenerHooks/Hook;)V
 */
JNIEXPORT void JNICALL Java_net_natpad_brilliance_chameleon_Chameleon_documentHookListener
  (JNIEnv *env, jclass cls, jlong document_id, jobject hook) {
	ChaDocument *document = CHA_DOCUMENT(document_id);
	BriChaDocumentListener *listener = bri_cha_document_listener_new(env, hook);
	cha_document_add_listener(document, (ChaIDocumentListener *) listener);
}


/*
 * Class:     net_natpad_brilliance_chameleon_Chameleon
 * Method:    createRevisionReader
 * Signature: (Lnet/natpad/brilliance/chameleon/Revision;)J
 */
JNIEXPORT jlong JNICALL Java_net_natpad_brilliance_chameleon_Chameleon_createRevisionReader
  (JNIEnv *env, jclass cls, jobject revision_obj) {
	ChaRevisionWo *revision = brilliance_as_object(env, revision_obj);
	return (jlong) cha_revision_reader_new(revision, NULL, NULL);
}

/*
 * Class:     net_natpad_brilliance_chameleon_Chameleon
 * Method:    revisionReaderScanUtf8Char
 * Signature: (Lnet/natpad/brilliance/chameleon/RevisionReader;)I
 */
JNIEXPORT jint JNICALL Java_net_natpad_brilliance_chameleon_Chameleon_revisionReaderScanUtf8Char
  (JNIEnv *env, jclass cls, jobject revision_reader_obj) {
	cat_log_debug("revision_reader_obj=%p", revision_reader_obj);
	ChaRevisionReader *revision_reader = brilliance_as_object(env, revision_reader_obj);
	cat_log_debug("revision_reader=%p", revision_reader);
	CatStreamStatus status;
	return (jint) cat_iutf8_scanner_next_char((CatIUtf8Scanner *) revision_reader, &status);
}


/*
 * Class:     net_natpad_brilliance_chameleon_Chameleon
 * Method:    createDocumentManager
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_net_natpad_brilliance_chameleon_Chameleon_createDocumentManager(JNIEnv *env, jclass cls) {
	return (jlong) cha_document_manager_new();
}

/*
 * Class:     net_natpad_brilliance_chameleon_Chameleon
 * Method:    createDocumentView
 * Signature: ()J
 */
JNIEXPORT jlong JNICALL Java_net_natpad_brilliance_chameleon_Chameleon_createDocumentView
  (JNIEnv *env, jclass cls) {
	return (jlong) bri_cha_document_view_new();
}

/*
 * Class:     net_natpad_brilliance_chameleon_Chameleon
 * Method:    constructDocumentView
 * Signature: (Lnet/natpad/brilliance/chameleon/DocumentView;Lnet/natpad/brilliance/chameleon/Document;JLnet/natpad/brilliance/gtk/GtkWidget;)J
 */
JNIEXPORT void JNICALL Java_net_natpad_brilliance_chameleon_Chameleon_constructDocumentView
  (JNIEnv *env, jclass cls, jobject document_view_obj, jobject document_obj, jlong pango_context_obj, jobject widget_obj) {

	ChaDocumentView *document_view = brilliance_as_object(env, document_view_obj);
	ChaDocument *document = brilliance_as_object(env, document_obj);
	PangoContext *pango_context = (PangoContext *) pango_context_obj;
	GtkWidget *widget = brilliance_as_object(env, widget_obj);

	if (BRI_IS_CHA_DOCUMENT_VIEW(document_view)) {
		bri_cha_document_view_construct((BriChaDocumentView *) document_view, env, document_view_obj, document, pango_context, widget);
	}
}

/*
 * Class:     net_natpad_brilliance_chameleon_Chameleon
 * Method:    documentViewSetPlainSelection
 * Signature: (Lnet/natpad/brilliance/chameleon/DocumentView;Lnet/natpad/brilliance/chameleon/Cursor;Lnet/natpad/brilliance/chameleon/Cursor;)V
 */
JNIEXPORT void JNICALL Java_net_natpad_brilliance_chameleon_Chameleon_documentViewSetPlainSelection
  (JNIEnv *env, jclass cls, jobject document_view_obj, jobject start_cursor_obj, jobject end_cursor_obj) {

	ChaCursorWo *start_cursor = bri_cha_cursor_wo_from_java(env, start_cursor_obj);
	ChaCursorWo *end_cursor = bri_cha_cursor_wo_from_java(env, end_cursor_obj);

	ChaDocumentView *document_view = brilliance_as_object(env, document_view_obj);
	cha_document_view_set_plain_selection(document_view, start_cursor, end_cursor);
	cat_unref(start_cursor);
	cat_unref(end_cursor);
}


/*
 * Class:     net_natpad_brilliance_chameleon_Chameleon
 * Method:    getDocumentView
 * Signature: (Lnet/natpad/brilliance/chameleon/ChameleonEditor;)Lnet/natpad/brilliance/chameleon/DocumentView;
 */
JNIEXPORT jobject JNICALL Java_net_natpad_brilliance_chameleon_Chameleon_editorGetDocumentView
  (JNIEnv *env, jclass cls, jobject editor_obj) {
	ChaEditor *editor = brilliance_as_object(env, editor_obj);
	ChaDocumentView *document_view = cha_editor_get_document_view(editor);
	cat_log_debug("document_view=%O", document_view);
	if (BRI_IS_CHA_DOCUMENT_VIEW(document_view)) {
		return bri_cha_document_view_get_link((BriChaDocumentView *) document_view, env);
	} else if (BRI_IS_DRA_DOCUMENT_VIEW(document_view)) {
		return bri_dra_document_view_get_link((BriDraDocumentView *) document_view, env);
	}
	return NULL;
}

/*
 * Class:     net_natpad_brilliance_chameleon_Chameleon
 * Method:    editorPostUowInsertText
 * Signature: (Lnet/natpad/brilliance/chameleon/ChameleonEditor;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_net_natpad_brilliance_chameleon_Chameleon_editorPostUowInsertText
  (JNIEnv *env, jclass cls, jobject editor_obj, jstring text_str) {
	ChaEditor *editor = brilliance_as_object(env, editor_obj);

	const char* chrs = (*env)->GetStringUTFChars(env, text_str, NULL);
	int len = (*env)->GetStringUTFLength(env, text_str);
	CatStringWo *t = cat_string_wo_new_with_len(chrs, len);

	ChaUow *uow = (ChaUow *) cha_uow_insert_text_new(t);
	cha_editor_run(editor, uow);
	cat_unref_ptr(uow);
	(*env)->ReleaseStringUTFChars(env, text_str, chrs);
}

/*
 * Class:     net_natpad_brilliance_chameleon_Chameleon
 * Method:    documentGetCurrentRevision
 * Signature: (Lnet/natpad/brilliance/chameleon/Document;)Lnet/natpad/brilliance/chameleon/Revision;
 */
JNIEXPORT jobject JNICALL Java_net_natpad_brilliance_chameleon_Chameleon_documentGetCurrentRevision
  (JNIEnv *env, jclass cls, jobject document_obj) {
	ChaDocument *document = brilliance_as_object(env, document_obj);
	ChaRevisionWo *revision = cha_document_get_current_revision_ref(document);
	jobject result = NULL;
	if (revision) {
		jclass revision_reader_cls = (*env)->FindClass(env, "net/natpad/brilliance/chameleon/Revision");
		if (revision_reader_cls == NULL) {
			cat_log_error("Class not found:Revision");
			return NULL;
		}
		jmethodID mod_d = (*env)->GetMethodID(env, revision_reader_cls, "<init>", "(J)V");
		result = (*env)->NewObject(env, revision_reader_cls, mod_d, (jlong) revision);
	}
	return result;
}


/*
 * Class:     net_natpad_brilliance_chameleon_Chameleon
 * Method:    pageHoldLines
 * Signature: (Lnet/natpad/brilliance/chameleon/Page;)V
 */
JNIEXPORT void JNICALL Java_net_natpad_brilliance_chameleon_Chameleon_pageHoldLines
  (JNIEnv *env, jclass cls, jobject page_obj) {
	ChaPageWo *page = brilliance_as_object(env, page_obj);
	cha_page_wo_hold_lines(page);
}

/*
 * Class:     net_natpad_brilliance_chameleon_Chameleon
 * Method:    pageReleaseLines
 * Signature: (Lnet/natpad/brilliance/chameleon/Page;)V
 */
JNIEXPORT void JNICALL Java_net_natpad_brilliance_chameleon_Chameleon_pageReleaseLines
  (JNIEnv *env, jclass cls, jobject page_obj) {
	ChaPageWo *page = brilliance_as_object(env, page_obj);
	cha_page_wo_release_lines(page);
}

/*
 * Class:     net_natpad_brilliance_chameleon_Chameleon
 * Method:    pageLineAt
 * Signature: (Lnet/natpad/brilliance/chameleon/Page;I)Lnet/natpad/brilliance/chameleon/Line;
 */
JNIEXPORT jobject JNICALL Java_net_natpad_brilliance_chameleon_Chameleon_pageLineAt
  (JNIEnv *env, jclass cls, jobject page_obj, jint page_line_index) {
	ChaPageWo *page = brilliance_as_object(env, page_obj);
	ChaLineWo *line = cha_page_wo_line_at(page, page_line_index);
	jobject result = NULL;
	if (line) {
		jclass line_cls = (*env)->FindClass(env, "net/natpad/brilliance/chameleon/Line");
		if (line_cls == NULL) {
			cat_log_error("Class not found:Line");
			return NULL;
		}
		jmethodID mod_d = (*env)->GetMethodID(env, line_cls, "<init>", "(J)V");
		result = (*env)->NewObject(env, line_cls, mod_d, (jlong) line);
		cat_ref_ptr(line);
	}
	return result;
}

/*
 * Class:     net_natpad_brilliance_chameleon_Chameleon
 * Method:    lineGetText
 * Signature: (Lnet/natpad/brilliance/chameleon/Line;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_net_natpad_brilliance_chameleon_Chameleon_lineGetText
  (JNIEnv *env, jclass cls, jobject line_obj) {
	ChaLineWo *line = brilliance_as_object(env, line_obj);

	jstring result = NULL;
	CatStringWo *text = cha_line_wo_get_text(line);
	if (text) {
		result = (*env)->NewStringUTF(env, cat_string_wo_getchars(text));
	}
	return result;
}


/*
 * Class:     net_natpad_brilliance_chameleon_Chameleon
 * Method:    editorGotoLine
 * Signature: (Lnet/natpad/brilliance/chameleon/ChameleonEditor;J)Z
 */
JNIEXPORT jboolean JNICALL Java_net_natpad_brilliance_chameleon_Chameleon_editorGotoLine
  (JNIEnv *env, jclass cls, jobject editor_obj, jlong row) {
	ChaEditor *editor = brilliance_as_object(env, editor_obj);
	gboolean result = cha_editor_goto_line(editor, row);
	return result ? JNI_TRUE : JNI_FALSE;
}

/*
 * Class:     net_natpad_brilliance_chameleon_Chameleon
 * Method:    documentManagerLoad
 * Signature: (Lnet/natpad/brilliance/chameleon/DocumentManager;Lnet/natpad/brilliance/chameleon/Document;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_net_natpad_brilliance_chameleon_Chameleon_documentManagerLoad
  (JNIEnv *env, jclass cls, jobject document_manager_obj, jobject document_obj, jstring path_str) {
	ChaDocumentManager *document_manager = brilliance_as_object(env, document_manager_obj);
	ChaDocument *document = brilliance_as_object(env, document_obj);

	const char* chrs = (*env)->GetStringUTFChars(env, path_str, NULL);
	int len = (*env)->GetStringUTFLength(env, path_str);
	GFile *file = g_file_new_for_path(chrs);
	cha_document_manager_load(document_manager, document, file);
}
