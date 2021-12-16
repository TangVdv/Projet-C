#include <stdio.h>
#include <gtk/gtk.h>
#include <sqlite3.h>
#include <string.h>

GtkApplication *app;
GtkWidget *window;
GtkBuilder *builder;
GtkWidget *button;
sqlite3 *db;
sqlite3_stmt *res;

int callback(void *NotUsed, int rowCount, char **rowValue, char **rowName) {
    char buffer[100];
    char *num;
    int i, j;
    for (j = 1; j<3; j++) {
        sprintf(num, "%d", j);
        strcat(strcpy(buffer, "Day_"), num);
        printf("%s\n", buffer);
        button = GTK_WIDGET(gtk_builder_get_object(builder, buffer));
        if (button == NULL) return 1;
        for (i = 0; i < rowCount; i++) {
            printf("%s = %s\n", rowName[i], rowValue[i]);
        }
        printf("\n");
    }

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

    gtk_init(&argc, &argv);

    builder = gtk_builder_new_from_file("Test.glade");

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));

    app = gtk_application_new ("test.gtk.calendar", G_APPLICATION_FLAGS_NONE);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_builder_connect_signals(builder, NULL);

    char *sql = "select * from Account";
    rc = sqlite3_exec(db, sql, callback, 0, &err_msg);

    gtk_widget_show(window);

    gtk_main();

    sqlite3_finalize(res);
    sqlite3_close(db);
    
    return 0;
}
