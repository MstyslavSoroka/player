#include <glib/gstdio.h>
#include <gtk/gtk.h>

static void on_quit_clicked(GtkButton *button, gpointer user_data) {
  GtkWindow *window = GTK_WINDOW(user_data);
  gtk_window_destroy(window);
}

static void activate(GtkApplication *app, gpointer user_data) {
  GtkBuilder *builder = gtk_builder_new();

  GError *error = NULL;
  gtk_builder_add_from_file(builder, "app.ui", &error);

  GtkWindow *window = GTK_WINDOW(gtk_builder_get_object(builder, "window"));
  GtkWidget *quit_btn = GTK_WIDGET(gtk_builder_get_object(builder, "quit"));
  gtk_window_set_application(window, app);
  gtk_window_set_title(window, "Player");
  g_signal_connect(quit_btn, "clicked", G_CALLBACK(on_quit_clicked), window);
  gtk_widget_set_size_request(GTK_WIDGET(window), 600, 400);
  gtk_window_set_default_size(window, 1200, 800);
  gtk_window_present(window);
  g_object_unref(builder);
}

int main(int argc, char **argv) {
#ifdef GTK_SRCDIR
  g_chdir(GTK_SRCDIR);
#endif

  GtkApplication *app;
  int status;
  app = gtk_application_new("org.player.example", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return status;
}
