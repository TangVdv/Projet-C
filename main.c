#include <stdio.h>
#include <gtk/gtk.h>
#include <sqlite3.h>
#include <string.h>
#include <curl/curl.h>

GtkApplication *app;
GtkWidget *window;
GtkBuilder *builder;
GtkWidget *calendar_frame;
GtkWidget *dialog;
GtkWidget *entry;
GtkWidget *fileChooser;
GtkTreeIter iter;
GtkTreePath *path;
GtkListStore *ListStore;
sqlite3 *db;
sqlite3_stmt *res;
FILE *save_file;
CURL *curl;
char *err_msg = 0;
int rc;
const gchar *btn_calendar_id;

int year, month_id;
char month_name[12][10] = {
        "Janvier",
        "Février",
        "Mars",
        "Avril",
        "Mai",
        "Juin",
        "Juillet",
        "Août",
        "Septembre\0",
        "Octobre",
        "Novembre",
        "Décembre"
};

void    on_btn_delete_clicked(GtkWidget *b);
void    on_btn_overview_clicked(GtkWidget *b);
void    on_btn_import_clicked();

char * build_sql(const char *part_A, const char *part_B) {
    size_t size = strlen(part_A) + strlen(part_B);
    char *sql_final = malloc(size + 1);
    strcpy(sql_final, part_A);
    strcat(sql_final, part_B);
    return sql_final;
}

/*
 * Importation d'un calendrier :
 * Lis les valeurs d'un fichier texte provenant d'un autre calendrier pour les sauvegarder dans la base de donnée
 */
int import(){
    char line[100];
    while ( fgets(line, 100, save_file) != NULL) { // Tant que la ligne lu dans le fichier n'est pas nulle
        char * sql;
        sql = (char *) malloc(100 *sizeof(char)); // Allocation d'une variable
        strcpy(sql, "INSERT INTO Day(content, date) VALUES("); // Ajoute le début de la requête sql dans la variable
        strncat(sql, line, strlen(line)-1); // Ajoute la ligne lu dans le fichier texte à la suite de la requête
        strncat(sql, ")", 100); // Ajoute la parenthèse fermante de la requête sql
        sqlite3_exec(db, sql, NULL , 0, &err_msg); // Execute la requête sql
        free(sql);
    }
    fclose(save_file);
    return 0;
}

/*
 * Exportation d'un calendrier :
 * Récupère les valeurs dans la base de donnée pour les insérer dans un fichier texte
 */
int export_callback(void *NotUsed, int rowCount, char **rowValue, char **rowName) {
    for (int i = 1; i < rowCount; i++) {
        //printf("%s = %s\n", rowName[i], rowValue[i]);
        fputs("'", save_file); // Ouvre la guillemet au début d'une valeur
        fputs(rowValue[i], save_file); // Ajoute la valeur au fichier texte
        fputs("'", save_file); // Ferme la guillemet à la fin d'une valeur
        if (i != rowCount - 1) fputs(",", save_file); // Ajoute une virgule entre chaque valeur
    }
    fputs("\n", save_file); // Saute une ligne dans le fichier texte
    return 0;
}

int refresh_menu_callback(void *NotUsed, int rowCount, char **rowValue, char **rowName){
    GtkWidget *box_btn_calendar = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    GtkWidget *btn_calendar_overview = gtk_button_new_with_label(rowValue[1]);
    GtkWidget *btn_calendar_delete = gtk_button_new_with_label("delete");

    gtk_widget_set_name(btn_calendar_overview, rowValue[0]);
    gtk_widget_set_name(btn_calendar_delete, rowValue[0]);

    g_signal_connect(G_OBJECT(btn_calendar_overview), "clicked", G_CALLBACK(on_btn_overview_clicked), NULL);
    g_signal_connect(G_OBJECT(btn_calendar_delete), "clicked", G_CALLBACK(on_btn_delete_clicked), NULL);


    gtk_box_pack_start(GTK_BOX(box_btn_calendar),btn_calendar_overview,1,1,0);
    gtk_box_pack_start(GTK_BOX(box_btn_calendar),btn_calendar_delete,0,0,0);

    gtk_container_add (GTK_CONTAINER (calendar_frame), box_btn_calendar);
    gtk_widget_set_hexpand(box_btn_calendar, TRUE);
    gtk_widget_set_vexpand(box_btn_calendar, TRUE);
    gtk_widget_show_all(window);

    return 0;
}

void refresh_menu(){
    GList *children, *iter;
    // TODO : Understand and comment this shit
    children = gtk_container_get_children(GTK_CONTAINER(calendar_frame));
    for(iter = children; iter != NULL; iter = g_list_next(iter))
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    g_list_free(children);

    char *sql = "SELECT * FROM Calendar"; // Création d'une requête sql
    sqlite3_exec(db, sql, refresh_menu_callback, 0, &err_msg); // Execute la requête sql et envoie le résultat à la fonction "refresh_menu_callback"
}

int print_combobox(void *NotUsed, int rowCount, char **rowValue, char **rowName){
    gtk_list_store_append(ListStore, &iter);
    gtk_list_store_set(ListStore, &iter, 0, rowValue[0], -1);
}

void main_menu(){
    printf("Show menu\n");

    builder = gtk_builder_new_from_file("Test.glade");

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    gtk_window_set_default_size(GTK_WINDOW(window), 1000, 500);
    gtk_widget_show_all(window);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_builder_connect_signals(builder, NULL);

    calendar_frame = GTK_WIDGET(gtk_builder_get_object(builder, "Calendar_btn_grid"));
    ListStore = GTK_LIST_STORE(gtk_builder_get_object(builder, "ListStore"));
    refresh_menu();

    char *sql = "select name from Calendar"; // Création d'une requête sql
    sqlite3_exec(db, sql, print_combobox, 0, &err_msg); // Execute la requête sql et envoie le résultat à la fonction "print_combobox"

}



// TODO : Libcurl function ( don't understand anything from this **** )
size_t got_data(char *buffer, size_t itemsize, size_t nitems, void* ignorethis){
    size_t bytes = itemsize * nitems;
    int linenumber = 0;
    //printf("New chunk (%zu bytes)\n", bytes);
    //printf("%d\t", linenumber);
    for (int i = 0; i < bytes; ++i) {

        if (linenumber == 20){
            printf("%c", buffer[i]);
            if (buffer[i] == '\n') break;
        }

        else {
            if (buffer[i] == '\n') {
                linenumber++;
                //printf("%d:\t", linenumber);
            }
        }
        /*
        printf("%c", buffer[i]);
        if (buffer[i] == '\n') {
            linenumber++;
            printf("%d:\t", linenumber);
        }
         */
    }
    printf("\n");
    return bytes;
}

int print_label_calendar_name(void *NotUsed, int rowCount, char **rowValue, char **rowName){
    GtkWidget *label_calendar_name = GTK_WIDGET(gtk_builder_get_object(builder, "label_calendar_name"));
    printf("%s", rowValue[0]);
    gtk_label_set_text(GTK_LABEL(label_calendar_name), rowValue[0]);
    gtk_widget_show_all(window);
}

void refresh_calendar(){
    char snum[4];
    GtkWidget *label_year = GTK_WIDGET(gtk_builder_get_object(builder, "label_year"));
    GtkWidget *label_month = GTK_WIDGET(gtk_builder_get_object(builder, "label_month"));


    sprintf(snum,"%d",year);
    gtk_label_set_text(GTK_LABEL(label_year), snum);
    gtk_label_set_text(GTK_LABEL(label_month), month_name[month_id]);

    gtk_widget_show_all(window);
}

void main_calendar() {

    year = 2021;
    month_id = 11;

    /*
    curl = curl_easy_init();
    if(curl){
        curl_easy_setopt(curl, CURLOPT_URL, "https://www.calendardate.com/todays.htm");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, got_data);

        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    */

    builder = gtk_builder_new_from_file("Calendar.glade");

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window_calendar"));

    refresh_calendar();

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_builder_connect_signals(builder, NULL);
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
    gtk_init(&argc, &argv);

    main_menu();

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

void    on_import_activate(){
    dialog = GTK_WIDGET(gtk_builder_get_object(builder, "dialog_import"));
    gtk_window_set_default_size(GTK_WINDOW(dialog), 400, 300);
    gtk_widget_show(dialog);
}

void    on_export_activate(){
    dialog = GTK_WIDGET(gtk_builder_get_object(builder, "dialog_export"));
    gtk_window_set_default_size(GTK_WINDOW(dialog), 400, 300);
    gtk_widget_show(dialog);

}

void    on_exit_activate(){
    gtk_main_quit();
}


int check_login_exist_callback(void *NotUsed, int rowCount, char **rowValue, char **rowName){
    //if(rowValue[0] == "1")
        printf("%s", rowValue[0]);


}

void    on_login_activate(){
    dialog = GTK_WIDGET(gtk_builder_get_object(builder, "dialog_login"));
    gtk_window_set_default_size(GTK_WINDOW(dialog), 400, 300);
    gtk_widget_show(dialog);

}

void    on_register_activate(){
    dialog = GTK_WIDGET(gtk_builder_get_object(builder, "dialog_register"));
    gtk_window_set_default_size(GTK_WINDOW(dialog), 400, 300);
    gtk_widget_show(dialog);

}



void    on_btn_cancel_clicked(){
    gtk_widget_hide(dialog);
}

void    on_btn_overview_clicked(GtkWidget *b){
    gtk_widget_hide(window);
    main_calendar();

    btn_calendar_id =   gtk_widget_get_name(b);
    char * sql;
    sql = (char *) malloc(100 *sizeof(char)); // Allocation d'une variable
    strcpy(sql, "SELECT name FROM Calendar WHERE id="); // Ajoute le début de la requête sql dans la variable
    strcat(sql, btn_calendar_id);
    sqlite3_exec(db, sql, print_label_calendar_name, 0, &err_msg); // Execute la requête sql
    free(sql);

}

void    on_btn_delete_clicked(GtkWidget *b){
    btn_calendar_id =   gtk_widget_get_name(b);
    char * sql;
    sql = (char *) malloc(100 *sizeof(char)); // Allocation d'une variable
    strcpy(sql, "DELETE FROM Calendar WHERE id="); // Ajoute le début de la requête sql dans la variable
    strcat(sql, btn_calendar_id);
    sqlite3_exec(db, sql, NULL, 0, &err_msg); // Execute la requête sql
    free(sql);
    refresh_menu();
}

void    on_btn_create_clicked(){
    entry = GTK_WIDGET(gtk_builder_get_object(builder, "entry_new_name"));
    char *getEntry = gtk_editable_get_chars(GTK_EDITABLE(entry), 0, -1);
    if (strcmp(getEntry, "") != 0) {
        char * sql;
        sql = (char *) malloc(100 *sizeof(char)); // Allocation d'une variable
        strcpy(sql, "INSERT INTO Calendar(name) VALUES('"); // Ajoute le début de la requête sql dans la variable
        strcat(sql, getEntry); // Ajoute la valeur dans la requête sql
        strcat(sql, "')"); // Ajoute la parenthèse fermante de la requête sql

        sqlite3_exec(db, sql, NULL, 0,&err_msg); // Execute la requête sql et envoie le résultat à la fonction "refresh_menu"
        free(sql);
        refresh_menu();

        gtk_editable_delete_text(GTK_EDITABLE(entry), 0, -1);
        on_btn_cancel_clicked();
    }
}

void    on_btn_validate_import_clicked(){
    fileChooser = GTK_WIDGET(gtk_builder_get_object(builder, "fileChooser_import"));
    char *getFileName = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(fileChooser));
    if (strcmp(getFileName, "") != 0) {
        //printf("file name : %s\n", getFileName);
        save_file = fopen(getFileName, "rt");  // Ouvre le fichier texte en mode lecture
        import();
    }
    on_btn_cancel_clicked();
}

void    on_btn_validate_export_clicked(){
    entry = GTK_WIDGET(gtk_builder_get_object(builder, "entry_export_name"));
    fileChooser = GTK_WIDGET(gtk_builder_get_object(builder, "fileChooser_export"));
    char *getFolderName = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(fileChooser));
    char *getEntry = gtk_editable_get_chars(GTK_EDITABLE(entry), 0, -1);
    if (strcmp(getFolderName, "") != 0 && strcmp(getEntry, "") != 0) {
        char *getFileName;
        printf("%s\n", getEntry);
        getFileName = (char *) malloc(100 * sizeof(char));
        strcpy(getFileName, getFolderName);
        strcat(getFileName, "/");
        strcat(getFileName, getEntry);
        strcat(getFileName, ".txt");
        on_btn_cancel_clicked();

        //printf("file name : %s\n", getFileName);
        save_file = fopen(getFileName, "w"); // Créer le fichier s'il n'existe pas et l'ouvre en mode édition
        //     TODO: Changer la requête sql
        char *sql = "select * from Day"; // Création d'une requête sql
        sqlite3_exec(db, sql, export_callback, 0, &err_msg); // Execute la requête sql et envoie le résultat à la fonction "export_callback"
        free(getFileName);
        fclose(save_file);
    }
}

void    on_btn_login_clicked(){
    entry = GTK_WIDGET(gtk_builder_get_object(builder, "entry_name_login"));
    char *getEntry_username = gtk_editable_get_chars(GTK_EDITABLE(entry), 0, -1);
    if (strcmp(getEntry_username, "") == 0) return;
    entry = GTK_WIDGET(gtk_builder_get_object(builder, "entry_password_login"));
    char *getEntry_password = gtk_editable_get_chars(GTK_EDITABLE(entry), 0, -1);
    if (strcmp(getEntry_password, "") == 0) return;

    char * sql = build_sql(
                    build_sql(
                        build_sql(
                            build_sql(
                            "SELECT COUNT() FROM Account WHERE name='", getEntry_username),
                        "'AND password='"),
                    getEntry_password),
                 "'");

    printf("%sql : %s", sql);
    sqlite3_exec(db, sql, check_login_exist_callback, 0,&err_msg); // Execute la requête sql et envoie le résultat à la fonction "refresh_menu"
    refresh_menu();

    on_btn_cancel_clicked();
}

void    on_btn_register_clicked(){
    entry = GTK_WIDGET(gtk_builder_get_object(builder, "entry_new_name"));
    char *getEntry = gtk_editable_get_chars(GTK_EDITABLE(entry), 0, -1);
    if (strcmp(getEntry, "") != 0) {
        char * sql;
        sql = (char *) malloc(100 *sizeof(char)); // Allocation d'une variable
        strcpy(sql, "INSERT INTO Calendar(name) VALUES('"); // Ajoute le début de la requête sql dans la variable
        strcat(sql, getEntry); // Ajoute la valeur dans la requête sql
        strcat(sql, "')"); // Ajoute la parenthèse fermante de la requête sql

        sqlite3_exec(db, sql, NULL, 0,&err_msg); // Execute la requête sql et envoie le résultat à la fonction "refresh_menu"
        free(sql);
        refresh_menu();

        gtk_editable_delete_text(GTK_EDITABLE(entry), 0, -1);
        on_btn_cancel_clicked();
    }
}


void    on_btn_menu_calendar_clicked(){
    gtk_widget_hide(window);
    main_menu();
}

void    on_btn_next_year_clicked(){
    year++;
    refresh_calendar();
}
void    on_btn_previous_year_clicked(){
    year--;
    refresh_calendar();
}
void    on_btn_next_month_clicked(){
    if(month_id == 11){
        year++;
        month_id = 0;
    }
    else
        month_id++;

    refresh_calendar();
}
void    on_btn_previous_month_clicked(){
    if(month_id == 0){
        year--;
        month_id = 11;
    }
    else
        month_id--;

    refresh_calendar();
}