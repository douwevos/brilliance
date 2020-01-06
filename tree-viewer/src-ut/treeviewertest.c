/*
   File:    treeviewertest.c
   Project: tree-viewer
   Author:  Douwe Vos
   Date:    Oct 1, 2019
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


/*
   File:    chameleontest.c
   Project: chameleon
   Author:  Douwe Vos
   Date:    Jan 22, 2015
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2015 Douwe Vos.

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

#include <gtk/gtk.h>
#include "britreenodefile.h"
#include "../src/britreeviewer.h"
#include "../src/model/britreemodel.h"


#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "BriTreeViewer"
#include <logging/catlog.h>

static gboolean l_close_window(GtkWidget *widget, GdkEvent *eev, gpointer main_ptr) {
	gtk_main_quit();
	return FALSE;
}


static void my_g_log_default_handler(const gchar *log_domain, GLogLevelFlags log_level, const gchar *message, gpointer user_data) {
	cat_log_fatal("%s(level:%d) %s", log_domain, log_level, message);
	cat_stacktrace_print();

#ifdef NATPAD_TESTING

#ifdef MONITOR_CLAZZ
	cat_ref_report();
	cat_ref_summary();
	cat_ref_cleanup();
#endif

	cat_ref_entry_report();
#endif
}

int main(int argc, char **argv) {
	g_log_set_handler("GLib-GObject", G_LOG_LEVEL_CRITICAL, (GLogFunc) my_g_log_default_handler, NULL);
	g_log_set_handler("GLib-GObject", G_LOG_LEVEL_WARNING, (GLogFunc) my_g_log_default_handler, NULL);
	g_log_set_handler("Gdk", G_LOG_LEVEL_CRITICAL, (GLogFunc) my_g_log_default_handler, NULL);
	g_log_set_handler("Gdk", G_LOG_LEVEL_WARNING, (GLogFunc) my_g_log_default_handler, NULL);
	g_log_set_handler("Gtk", G_LOG_LEVEL_CRITICAL, (GLogFunc) my_g_log_default_handler, NULL);
	g_log_set_handler("Gtk", G_LOG_LEVEL_WARNING, (GLogFunc) my_g_log_default_handler, NULL);
	g_log_set_handler(NULL, G_LOG_LEVEL_CRITICAL, (GLogFunc) my_g_log_default_handler, NULL);
	g_log_set_handler(NULL, G_LOG_LEVEL_WARNING, (GLogFunc) my_g_log_default_handler, NULL);

	gtk_init(&argc, &argv);

//	ChaEditorService *editor_service = cha_editor_service_new();

	GtkWidget *top_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect(G_OBJECT(top_window), "delete-event", G_CALLBACK(l_close_window), NULL);
	gtk_widget_set_size_request(top_window, 512, 368);


	GtkScrolledWindow *scrolled_window = (GtkScrolledWindow *) gtk_scrolled_window_new(NULL, NULL);
	gtk_container_add((GtkContainer *) top_window, (GtkWidget *) scrolled_window);

	BriTreeNodeFile *tree_node_file = bri_tree_node_file_new();

	BriTreeModel *tree_model = bri_tree_model_new((BriTreeNode *) tree_node_file);
	BriTreeViewer *tree_viewer = bri_tree_viewer_new(tree_model);

	gtk_container_add((GtkContainer *) scrolled_window, (GtkWidget *) tree_viewer);

	BriTreeNodeRef *node_ref = bri_tree_model_get_root_ref(tree_model);
	bri_tree_node_ref_set_text(node_ref, cat_string_wo_new_with("Hallo"));



	gtk_widget_set_visible(top_window, TRUE);
	gtk_widget_show_all(top_window);
//	gtk_widget_grab_focus((GtkWidget *) cha_editor);
	gtk_main();




#ifdef NATPAD_TESTING

#ifdef MONITOR_CLAZZ
	cat_ref_report();
	cat_ref_summary();
	cat_ref_cleanup();
#endif

	cat_ref_entry_report();
#endif
	return 0;
}
