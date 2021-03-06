/*
   File:    brillianceapp.c
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

#include "brillianceapp.h"
#include "brilliancewindow.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_ALL
#define CAT_LOG_CLAZZ "BrillianceApp"
#include <logging/catlog.h>

struct _BrillianceAppPrivate {
	void *dummy;
};

G_DEFINE_TYPE_WITH_CODE(BrillianceApp, brilliance_app, GTK_TYPE_APPLICATION,  // @suppress("Unused static function")
	G_ADD_PRIVATE(BrillianceApp)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_app_startup(GApplication *application);
static void l_app_activate(GApplication *application);
static int l_app_command_line(GApplication *application, GApplicationCommandLine *cmd_line);
static gboolean l_app_local_command_line(GApplication *application, gchar ***arguments, gint *exit_status);

static void brilliance_app_class_init(BrillianceAppClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	GApplicationClass *app_class = G_APPLICATION_CLASS(clazz);
	app_class->startup = l_app_startup;
	app_class->activate = l_app_activate;
	app_class->command_line = l_app_command_line;
	app_class->local_command_line = l_app_local_command_line;
}

static void brilliance_app_init(BrillianceApp *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	BrillianceApp *instance = BRILLIANCE_APP(object);
//	BrillianceAppPrivate *priv = brilliance_app_get_instance_private(instance);
	G_OBJECT_CLASS(brilliance_app_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(brilliance_app_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


BrillianceApp *brilliance_app_new() {
	BrillianceApp *result = g_object_new(BRILLIANCE_TYPE_APP,
			"application-id", "net.natpad.Brilliance",
            "flags", G_APPLICATION_HANDLES_COMMAND_LINE,
			NULL);
	cat_ref_anounce(result);
	BrillianceAppPrivate *priv = brilliance_app_get_instance_private(result);
	return result;
}



static void l_app_startup(GApplication *application) {
	G_APPLICATION_CLASS(brilliance_app_parent_class)->startup(application);
}

static void l_app_activate(GApplication *application) {
	/* get current window */
	GtkWindow *current_window = NULL;
	GList *windows = gtk_application_get_windows(GTK_APPLICATION(application));
	if (windows) {
		current_window = (GtkWindow *) windows->data;
	}

	BrillianceAppPrivate *priv = brilliance_app_get_instance_private(BRILLIANCE_APP(application));
	if (current_window==NULL) {
//
//		VipCdProvider *cd_provider = vip_cd_provider_new();
//		ElkPreferencesService *preferences_service = elk_preferences_service_new(cd_provider);
//		ElkPreferencesWo *a_elk_prefs = elk_preferences_service_load(preferences_service);
//		if (a_elk_prefs==NULL) {
//			cat_log_debug("missing prefs");
//			a_elk_prefs = elk_preferences_wo_anchor(elk_preferences_wo_new(), 0);
//			ElkPreferencesContainer *prefs_container = elk_preferences_service_get_container(preferences_service);
//			elk_preferences_container_set(prefs_container, a_elk_prefs);
//			cat_unref(a_elk_prefs);
//		}

		current_window = (GtkWindow *) brilliance_window_new(application);
		gtk_widget_show(GTK_WIDGET(current_window));
	}

//	if (priv->file_list && cat_array_wo_size(priv->file_list)>0) {
//		brilliance_window_new_open_file_list((BrillianceWindow *) current_window, priv->file_list);
//	} else {
//		brilliance_window_new_open_empty_editor((BrillianceWindow *) current_window);
//	}

	gtk_window_present(GTK_WINDOW(current_window));
}

//static gchar **remaining_args;

static GOptionContext *l_create_option_context(BrillianceApp *application) {
	BrillianceAppPrivate *priv = brilliance_app_get_instance_private(application);

	GOptionContext *context = g_option_context_new("- Edit text files");
	g_option_context_set_help_enabled(context, FALSE);
	g_option_context_add_group(context, gtk_get_option_group(FALSE));

	GOptionEntry *entries = g_new0(GOptionEntry, 2);

	GOptionEntry *entry = &(entries[0]);

	entry->long_name = G_OPTION_REMAINING;
	entry->short_name = '\0';
	entry->flags = 0;
	entry->arg = G_OPTION_ARG_FILENAME_ARRAY;
	entry->arg_data = NULL;
//	entry->arg_data = &(priv->remaining_args);
	entry->description = NULL;
	entry->arg_description = "[FILE...]";

	g_option_context_add_main_entries(context, entries, NULL);
	return context;
}


static gchar** l_arguments_copy(gchar **arg, int arg_count) {
	gchar **opt_arguments = g_new(gchar *, arg_count);
	int idx;
	for (idx=0; idx<arg_count; idx++) {
		cat_log_debug("argje=%s", arg[idx]);

		opt_arguments[idx] = arg[idx];
	}
	return opt_arguments;
}

static int l_app_command_line(GApplication *application, GApplicationCommandLine *cmd_line) {
	BrillianceAppPrivate *priv = brilliance_app_get_instance_private(BRILLIANCE_APP(application));

	gchar **arguments = g_application_command_line_get_arguments(cmd_line, NULL);
	int arguments_count = g_strv_length(arguments);

	gchar **opt_arguments = l_arguments_copy(arguments, arguments_count);

	cat_log_detail("arg_count=%d, arg[1]=%s", arguments_count, opt_arguments[1]);
	GOptionContext *context = l_create_option_context((BrillianceApp *) application);

	GError *error = NULL;
	if (g_option_context_parse(context, &arguments_count, &opt_arguments, &error)) {
//		cat_log_debug("was successfull");
////		cat_unref_ptr(priv->file_list);
//		if (priv->remaining_args) {
//			cat_unref_ptr(priv->file_list);
//			priv->file_list = cat_array_wo_new();
//			int i;
//			for(i=0; priv->remaining_args[i]; i++) {
//				cat_log_debug("arg=%s", priv->remaining_args[i]);
//				GFile *file = g_application_command_line_create_file_for_arg(cmd_line, priv->remaining_args[i]);
//				cat_array_wo_append(priv->file_list, (GObject *) file);
//				cat_unref_ptr(file);
//			}
//		}
		g_application_activate(application);
	} else {
		cat_log_error("%s",error->message);
		g_application_command_line_printerr(cmd_line, "%s", error->message);
		g_error_free(error);
		g_application_command_line_set_exit_status (cmd_line, 1);
	}

	g_option_context_free(context);
	cat_free_ptr(opt_arguments);
//	g_strfreev(priv->remaining_args);
//	priv->remaining_args = NULL;

	g_strfreev(arguments);

	return 0;
}

static gboolean l_app_local_command_line(GApplication *application, gchar ***arguments, gint *exit_status) {
	BrillianceAppPrivate *priv = brilliance_app_get_instance_private(BRILLIANCE_APP(application));

	int arg_count = g_strv_length(*arguments);
	gchar **opt_arguments = l_arguments_copy(*arguments, arg_count);

	cat_log_debug("arg_count=%d", arg_count);

	GOptionContext *context = l_create_option_context((BrillianceApp *) application);
	GError *error = NULL;
	if (!g_option_context_parse(context, &arg_count, &opt_arguments, &error)) {
		cat_log_error("%s", error->message);
		g_error_free(error);
		*exit_status = 1;
	}

	gboolean result = G_APPLICATION_CLASS(brilliance_app_parent_class)->local_command_line(application, arguments, exit_status);

	g_option_context_free(context);
	cat_free_ptr(opt_arguments);
//	g_strfreev(priv->remaining_args);
//	priv->remaining_args = NULL;

	return result;
}







