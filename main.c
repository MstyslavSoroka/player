#include <glib/gstdio.h>
#include <gtk/gtk.h>
#include <sqlite3.h>


typedef struct {
    sqlite3 *db;
} AppData;


typedef struct {
    sqlite3 *db;
    GtkEntry *entry;
} CreatePlaylistData;

static void on_add_clicked(GtkButton *button, gpointer user_data) {
    GtkPopover *popover = GTK_POPOVER(user_data);
    gtk_popover_popup(popover);
  }

static void on_create_clicked(GtkButton *button, gpointer user_data)
{
      CreatePlaylistData *data = user_data;

    const char *name =
        gtk_editable_get_text(GTK_EDITABLE(data->entry));

    sqlite3_stmt *stmt;

    sqlite3_prepare_v2(
        data->db,
        "INSERT INTO playlists(name) VALUES(?);",
        -1,
        &stmt,
        NULL
    );

    sqlite3_bind_text(stmt, 1, name, -1, SQLITE_TRANSIENT);
    sqlite3_step(stmt);
     g_print("Created playlist: %s\n", name);
    sqlite3_finalize(stmt);
}

static void activate(GtkApplication *app, gpointer user_data) {
   AppData *app_data = user_data;

  GtkBuilder *builder = gtk_builder_new();

  GError *error = NULL;
  gtk_builder_add_from_file(builder, "app.ui", &error);

  GtkWindow *window = GTK_WINDOW(gtk_builder_get_object(builder, "window"));
  GtkWidget *add_playlist = GTK_WIDGET(gtk_builder_get_object(builder, "add_playlist"));

  gtk_window_set_application(window, app);
    gtk_window_set_title(window, "Player");
    gtk_window_set_default_size(window, 1200, 800);


  GtkWidget *playlist_window = gtk_popover_new();
  gtk_widget_set_parent(playlist_window, add_playlist);

  GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
    gtk_popover_set_child(GTK_POPOVER(playlist_window), box);

    gtk_box_append(GTK_BOX(box),
        gtk_label_new("Create new playlist"));
    
    GtkWidget *playlist_name = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(playlist_name), "Playlist name");
    GtkWidget *create = gtk_button_new_with_label("Create");
   


    gtk_box_append(GTK_BOX(box),playlist_name);
    gtk_box_append(GTK_BOX(box),create);

  CreatePlaylistData *create_data =
        g_new(CreatePlaylistData, 1);

   create_data->db = app_data->db;
    create_data->entry = GTK_ENTRY(playlist_name);
 
    g_signal_connect(
        create,
        "clicked",
        G_CALLBACK(on_create_clicked),
        create_data
    );

  g_signal_connect(add_playlist, "clicked", G_CALLBACK(on_add_clicked), playlist_window);
  gtk_window_present(window);
  g_object_unref(builder);
}

int main(int argc, char **argv) {
#ifdef GTK_SRCDIR
  g_chdir(GTK_SRCDIR);
#endif
 
  sqlite3 *db;
  if (sqlite3_open("./db/playlists.db", &db) != SQLITE_OK) {
    printf("Failed to open database: %s\n", sqlite3_errmsg(db));
    return 1;
  }

  char *err = NULL;

  sqlite3_exec(
    db,
    "CREATE TABLE IF NOT EXISTS playlists ("
    "id INTEGER PRIMARY KEY,"
    "name TEXT NOT NULL"
    ");",
    NULL,
    NULL,
    &err
  );

  AppData app_data = {
        .db = db
    };

GtkApplication *app;
int status;

app = gtk_application_new(
    "org.player.example",
    G_APPLICATION_DEFAULT_FLAGS
);

g_signal_connect(
    app,
    "activate",
    G_CALLBACK(activate),
    &app_data
);  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

 sqlite3_close(db);

  return status;
}
