#include <stdio.h>
#include <gtk/gtk.h>
#include <sqlite3.h>
#include <string.h>

GtkApplication *app;
GtkWidget *window;
GtkBuilder *builder;
GtkWidget *calendar_frame;
GtkWidget *btn_create;
GtkWidget *dialog;
sqlite3 *db;
sqlite3_stmt *res;
FILE *save_file;
char *err_msg = 0;
int rc;

void    on_btn_delete_clicked(GtkWidget *b);
void    on_btn_export_clicked(GtkWidget *b);
void    on_btn_import_clicked();

/*
 * Importation d'un calendrier :
 * Lis les valeurs d'un fichier texte provenant d'un autre calendrier pour les sauvegarder dans la base de donnée
 */
int import(){
    save_file = fopen("export.txt", "rt");  // Ouvre le fichier texte en mode lecture
    char line[100];
    while ( fgets(line, 100, save_file) != NULL) { // Tant que la ligne lu dans le fichier n'est pas nulle
        char * sql;
        sql = (char *) malloc(100 *sizeof(char)); // Allocation d'une variable
        strcpy(sql, "INSERT INTO Account(name, password, type) VALUES("); // Ajoute le début de la requête sql dans la variable
        strncat(sql, line, strlen(line)-1); // Ajoute la ligne lu dans le fichier texte à la suite de la requête
        strncat(sql, ")", 100); // Ajoute la parenthèse fermante de la requête sql
        printf("\n%s", sql); // Affiche la requête sql
        sqlite3_exec(db, sql, NULL , 0, &err_msg); // Execute la requête sql
    }
    fclose(save_file);
    return 0;
}

/*
 * Exportation d'un calendrier :
 * Récupère les valeurs dans la base de donnée pour les insérer dans un fichier texte
 */
int export_callback(void *NotUsed, int rowCount, char **rowValue, char **rowName) {
    save_file = fopen("export.txt", "a"); // Créer le fichier s'il n'existe pas et l'ouvre en mode édition
    for (int i = 1; i < rowCount; i++) {
        //printf("%s = %s\n", rowName[i], rowValue[i]);
        fputs("'", save_file); // Ouvre la guillemet au début d'une valeur
        fputs(rowValue[i], save_file); // Ajoute la valeur au fichier texte
        fputs("'", save_file); // Ferme la guillemet à la fin d'une valeur
        if (i != rowCount - 1) fputs(",", save_file); // Ajoute une virgule entre chaque valeur
    }
    fputs("\n", save_file); // Saute une ligne dans le fichier texte
    fclose(save_file);
    return 0;
}

int refresh_callback(void *NotUsed, int rowCount, char **rowValue, char **rowName){
    GtkWidget *box_btn_calendar_main = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    GtkWidget *box_btn_calendar_option = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget *btn_calendar_overview = gtk_button_new();
    GtkWidget *btn_calendar_delete = gtk_button_new();
    GtkWidget *btn_calendar_export = gtk_button_new();
    GtkWidget *label_btn_overview = gtk_label_new(rowValue[1]);
    GtkWidget *label_btn_delete = gtk_label_new("delete");
    GtkWidget *label_btn_export = gtk_label_new("export");
    gtk_widget_set_name(btn_calendar_overview, rowValue[0]);
    gtk_widget_set_name(btn_calendar_delete, rowValue[0]);
    gtk_widget_set_name(btn_calendar_export, rowValue[0]);

    g_signal_connect(G_OBJECT(btn_calendar_delete), "clicked", G_CALLBACK(on_btn_delete_clicked), NULL);
    g_signal_connect(G_OBJECT(btn_calendar_export), "clicked", G_CALLBACK(on_btn_export_clicked), NULL);

    gtk_box_pack_start(GTK_BOX(box_btn_calendar_main),btn_calendar_overview,1,1,0);
    gtk_box_pack_start(GTK_BOX(box_btn_calendar_main),box_btn_calendar_option,0,0,0);

    gtk_box_pack_start(GTK_BOX(box_btn_calendar_option),btn_calendar_delete,0,0,0);
    gtk_box_pack_start(GTK_BOX(box_btn_calendar_option),btn_calendar_export,0,0,0);

    gtk_container_add (GTK_CONTAINER (btn_calendar_delete), label_btn_delete);
    gtk_container_add (GTK_CONTAINER (btn_calendar_overview), label_btn_overview);
    gtk_container_add (GTK_CONTAINER (btn_calendar_export), label_btn_export);

    gtk_container_add (GTK_CONTAINER (calendar_frame), box_btn_calendar_main);

    gtk_widget_show_all(window);

    return 0;
}

void refresh(){
    GList *children, *iter;
    // TODO : Understand and comment this shit
    children = gtk_container_get_children(GTK_CONTAINER(calendar_frame));
    for(iter = children; iter != NULL; iter = g_list_next(iter))
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    g_list_free(children);

    char *sql = "SELECT * FROM Calendar"; // Création d'une requête sql
    sqlite3_exec(db, sql, refresh_callback, 0, &err_msg); // Execute la requête sql et envoie le résultat à la fonction "refresh_callback"
}

int main(int argc, char **argv){

    printf("Start\n\n");

    rc = sqlite3_open("Projet.database", &db); // Ouvre la base de donnée

    // Check si la base de donnée existe
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }

    // DEBUT SETUP GTK
    gtk_init(&argc, &argv);

    builder = gtk_builder_new_from_file("Test.glade");

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));

    app = gtk_application_new ("test.gtk.calendar", G_APPLICATION_FLAGS_NONE);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_builder_connect_signals(builder, NULL);
    // FIN SETUP GTK

    calendar_frame = GTK_WIDGET(gtk_builder_get_object(builder, "Calendar_btn_grid"));
    btn_create = GTK_WIDGET(gtk_builder_get_object(builder, "btn_create"));

    refresh();

    gtk_main();

    sqlite3_finalize(res); // Supprime le "statement" de la base de donnée
    sqlite3_close(db); // Ferme la base de donnée

    printf("\n\nEnd\n");

    return 0;
}


void    on_new_activate(){
    dialog = GTK_WIDGET(gtk_builder_get_object(builder, "dialog_new"));
    gtk_window_set_default_size(GTK_WINDOW(dialog), 400, 300);
    gtk_widget_show(dialog);
}

void    on_exit_activate(){
    gtk_main_quit();
}

void    on_btn_cancel_clicked(){
    gtk_widget_destroy(dialog);
}

void    on_btn_import_clicked(){
    printf("Import");
}

void    on_btn_export_clicked(GtkWidget *b){
    const gchar *btnId =   gtk_widget_get_name(b);
    //     TODO: Changer la requête sql
    char *sql = "select * from Day"; // Création d'une requête sql
    sqlite3_exec(db, sql, export_callback, 0, &err_msg); // Execute la requête sql et envoie le résultat à la fonction "export_callback"

}

void    on_btn_delete_clicked(GtkWidget *b){
    const gchar *btnId =   gtk_widget_get_name(b);
    char * sql;
    sql = (char *) malloc(100 *sizeof(char)); // Allocation d'une variable
    strcpy(sql, "DELETE FROM Calendar WHERE id="); // Ajoute le début de la requête sql dans la variable
    strcat(sql, btnId);
    sqlite3_exec(db, sql, NULL, 0, &err_msg); // Execute la requête sql
    refresh();
}

void    on_btn_create_clicked(){
    GtkWidget *entry = GTK_WIDGET(gtk_builder_get_object(builder, "entry_new_name"));
    char *getEntry = gtk_editable_get_chars(GTK_EDITABLE(entry), 0, -1);
    if (strcmp(getEntry, "") != 0) {
        char * sql;
        sql = (char *) malloc(100 *sizeof(char)); // Allocation d'une variable
        strcpy(sql, "INSERT INTO Calendar(name) VALUES('"); // Ajoute le début de la requête sql dans la variable
        strcat(sql, getEntry); // Ajoute la valeur dans la requête sql
        strcat(sql, "')"); // Ajoute la parenthèse fermante de la requête sql

        sqlite3_exec(db, sql, NULL, 0,&err_msg); // Execute la requête sql et envoie le résultat à la fonction "refresh"
        refresh();

        gtk_editable_delete_text(GTK_EDITABLE(entry), 0, -1);
        on_btn_cancel_clicked();
    }
}
