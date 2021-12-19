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
FILE *save_file;
char *err_msg = 0;
int rc;


int import_callback(void *NotUsed, int rowCount, char **rowValue, char **rowName) {
    return 0;
}

int import(){
    save_file = fopen("export.txt", "rt");
    char line[100];
    while ( fgets(line, 100, save_file) != NULL) {
        char * sql;
        sql = (char *) malloc(100 *sizeof(char));
        strcpy(sql, "INSERT INTO Account(name, password, type) VALUES(");
        strncat(sql, line, strlen(line)-1);
        strncat(sql, ")", 100);
        printf("\n%s", sql);
        sqlite3_exec(db, sql, import_callback , 0, &err_msg);
    }
    fclose(save_file);
    return 0;
}

int export_callback(void *NotUsed, int rowCount, char **rowValue, char **rowName) {
    for (int i = 1; i <= 3; i++) {
        printf("%s = %s\n", rowName[i], rowValue[i]);
        if (i != rowCount - 1) fputs("'", save_file);
        fputs(rowValue[i], save_file);
        if (i != rowCount - 1) {
            fputs("'", save_file);
            fputs(",", save_file);
        }
    }
    fputs("\n", save_file);


    printf("\n");

    return 0;
    fclose(save_file);
}

int main(int argc, char **argv){

    save_file = fopen("export.txt", "a");

    rc = sqlite3_open("Projet.database", &db);

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
    sqlite3_exec(db, sql, export_callback, 0, &err_msg);

    import();
    /*
    gtk_widget_show(window);

    gtk_main();
    */
    sqlite3_finalize(res);
    sqlite3_close(db);

    return 0;
}
