#include <stdio.h>
#include <gtk/gtk.h>
#include <sqlite3.h>

GtkApplication *app;
GtkWidget *window;
GtkBuilder *builder;
GtkWidget *fixed1;
GtkWidget *button1;
GtkWidget *label1;
sqlite3 *db;
sqlite3_stmt *res;

int callback(void *NotUsed, int argc, char **argv,
             char **azColName) {

    NotUsed = 0;

    for (int i = 0; i < argc; i++) {

        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }

    printf("\n");

    return 0;
}

int main(int argc, char **argv){

    char *err_msg = 0;

    int rc = sqlite3_open("Projet.database", &db);

    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    char *sql = "SELECT * FROM Test";

    rc = sqlite3_exec(db, sql, callback, 0, &err_msg);

    if (rc != SQLITE_OK ) {

        fprintf(stderr, "Failed to select data\n");
        fprintf(stderr, "SQL error: %s\n", err_msg);

        sqlite3_free(err_msg);
        sqlite3_close(db);

        return 1;
    }


    sqlite3_finalize(res);
    sqlite3_close(db);

    gtk_init(&argc, &argv);

    builder = gtk_builder_new_from_file("Test.glade");

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));

    app = gtk_application_new ("test.gtk.calendar", G_APPLICATION_FLAGS_NONE);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    fixed1 = GTK_WIDGET(gtk_builder_get_object(builder, "fixed1"));
    button1 = GTK_WIDGET(gtk_builder_get_object(builder, "button1"));
    label1 = GTK_WIDGET(gtk_builder_get_object(builder, "label1"));


    gtk_builder_connect_signals(builder, NULL);

    gtk_widget_show(window);

    gtk_main();
    
    return 0;
}


void on_button1_clicked (GtkButton *b){
    gtk_label_set_text(GTK_LABEL(label1), (const gchar*) "Hello World");
}