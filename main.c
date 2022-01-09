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
GtkWidget *fichierMenuItem;
GtkWidget *compteMenuItem;
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

char *getEntry_username, *getEntry_password;

int user_id = -1;
char str_id[10];
int user_type = -1;
int year, month_id;
char month_name[12][10] = {"Janvier","Février","Mars","Avril","Mai","Juin","Juillet","Août","Septembre\0","Octobre","Novembre","Décembre"};

char *username;
char *password;

void    on_btn_delete_clicked(GtkWidget *b);
void    on_btn_overview_clicked(GtkWidget *b);

void    on_login_activate();
void    on_btn_register_clicked();
void    on_exit_activate();



//  BUILD SQL
char * build_sql(const char *part_A, const char *part_B) {
    size_t size = strlen(part_A) + strlen(part_B);
    char *sql_final = malloc(size + 1);
    strcpy(sql_final, part_A);
    strcat(sql_final, part_B);
    return sql_final;
}


//  /MAIN MENU

int refresh_menu_callback(void *NotUsed, int rowCount, char **rowValue, char **rowName){
    printf("Calendrier %d %s", rowCount, rowValue[1]);

    return 0;
}

void refresh_menu(){


    if (user_id >= 0) {
        sprintf(str_id, "%d", user_id);
        char *sql = build_sql("SELECT * FROM Calendar WHERE user_id = ", str_id); // Création d'une requête sql
        sqlite3_exec(db, sql, refresh_menu_callback, 0,
                     &err_msg); // Execute la requête sql et envoie le résultat à la fonction "refresh_menu_callback"
    }
}

int print_combobox(void *NotUsed, int rowCount, char **rowValue, char **rowName){
    gtk_list_store_append(ListStore, &iter);
    gtk_list_store_set(ListStore, &iter, 0, rowValue[0], -1);
}

void main_menu(){
    int user_choice;
    //system("clear");
    printf("---Compte---\n");
    printf("(1) Se connecter\n");
    printf("(2) Créer un compte\n");
    printf("(3) Quitter\n\n\n\n\n\n\n\n\n");
    printf("Veuillez saisir l'une des options ci-dessus:");

    fflush(stdin);
    scanf("%d", &user_choice);
    if(user_choice == 1){
        on_login_activate();
    }else{
        if(user_choice == 2){
            on_btn_register_clicked();
        }else{
            on_exit_activate();
        }
    }



    //refresh_menu();

}

//  MAIN MENU\



//  /CALENDAR

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
    //printf("%s", rowValue[0]);
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

//  CALENDAR\



// MAIN
int main(int argc, char **argv){
    printf("Start\n\n");

    rc = sqlite3_open("Projet.database", &db); // Ouvre la base de donnée

    // Check si la base de donnée existe
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return 1;
    }


    main_menu();

    gtk_main();

    sqlite3_finalize(res); // Supprime le "statement" de la base de donnée
    sqlite3_close(db); // Ferme la base de donnée

    free(username);
    free(password);

    printf("\n\nEnd\n");

    return 0;
}


//  CANCEL
void    on_btn_cancel_clicked(){

}



// EXIT

void    on_exit_activate(){

}



// LOGOUT

void    on_logout_activate(){
    user_id = -1;
    user_type = -1;
    refresh_menu();
}


// HASH PASSWORD
// TODO: REGLER ERREUR DE SEGMENTATION

char *hash_pwd(char *password){
    size_t size = strlen(password);
    char *value = malloc(size + 1);
    char *hash_password = malloc(size + 1);
    strcpy(hash_password, "");
    for(int i = 0; i < strlen(password); i++) {
        sprintf(value, "%0x", password[i]);
        strncat(hash_password, value, size);
        sprintf(value, "%u", password[i]);
        strncat(hash_password, value, size);
    }
    return hash_password;
}


// /LOGIN

int check_login_exist_callback(void *NotUsed, int rowCount, char **rowValue, char **rowName) {
    if (rowCount == 0 ){
        printf("Incorrect");
        return 1;
    }
    else{
        user_id = atoi(rowValue[0]);
        user_type = atoi(rowValue[2]);
        refresh_menu();
    }
}

void login(){
    password = hash_pwd(password);

    char *sql = build_sql(
            build_sql(
                    build_sql(
                            build_sql(
                                    "SELECT id,name,type FROM Account GROUP BY id HAVING COUNT(id) = 1 AND name='", username),
                            "' AND password='"),
                    password),
            "'");

    sqlite3_exec(db, sql, check_login_exist_callback, 0,&err_msg); // Execute la requête sql et envoie le résultat à la fonction "refresh_menu"
    //on_btn_cancel_clicked();
}

void    on_login_activate(){
    username = malloc(24 * sizeof(char));
    password = malloc(24 * sizeof(char));

    system("clear");
    printf("Veuillez saisir votre nom d'utilisateur:");
    fflush(stdin);
    scanf("%s", username);

    printf("\nVeuillez saisir votre mot de passe:");
    fflush(stdin);
    scanf("%s", password);
    login();

}



//  LOGIN\



//  /REGISTER

int check_if_user_exist(void *NotUsed, int rowCount, char **rowValue, char **rowName){
    if (strcmp(rowValue[0], "0") == 0){
        char *sql = build_sql(
                build_sql(
                        build_sql(
                                build_sql(
                                        "INSERT INTO Account(name, password) VALUES('", username),
                                "','"),
                        password),
                "')");
        sqlite3_exec(db, sql, NULL, 0,&err_msg); // Execute la requête sql et envoie le résultat à la fonction "refresh_menu"
        login();
    }
    else {
        printf("user '%s' already exist\n", username);
        return 0;
    }
}

void    on_register_activate(){
    dialog = GTK_WIDGET(gtk_builder_get_object(builder, "dialog_register"));
    gtk_window_set_default_size(GTK_WINDOW(dialog), 400, 300);
    gtk_widget_show(dialog);
}

void    on_btn_register_clicked(){
    entry = GTK_WIDGET(gtk_builder_get_object(builder, "entry_name_register"));
    getEntry_username = gtk_editable_get_chars(GTK_EDITABLE(entry), 0, -1);
    if (strcmp(getEntry_username, "") == 0) return;
    gtk_editable_delete_text(GTK_EDITABLE(entry), 0, -1);
    entry = GTK_WIDGET(gtk_builder_get_object(builder, "entry_password_register"));
    getEntry_password = gtk_editable_get_chars(GTK_EDITABLE(entry), 0, -1);
    if (strcmp(getEntry_password, "") == 0) return;

    gtk_editable_delete_text(GTK_EDITABLE(entry), 0, -1);
    char *sql = build_sql(build_sql("SELECT COUNT() FROM Account WHERE name='", getEntry_username),"'");

    getEntry_password = hash_pwd(getEntry_password);

    sqlite3_exec(db, sql, check_if_user_exist, 0,&err_msg); // Execute la requête sql et envoie le résultat à la fonction "refresh_menu"

    on_btn_cancel_clicked();
}

//  REGISTER\



//  /EXPORT

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

void    on_export_activate(){
    dialog = GTK_WIDGET(gtk_builder_get_object(builder, "dialog_export"));
    gtk_window_set_default_size(GTK_WINDOW(dialog), 400, 300);
    sprintf(str_id, "%d", user_id);
    char *sql = build_sql(build_sql("SELECT name FROM Calendar WHERE user_id='", str_id),"'"); // Création d'une requête sql
    sqlite3_exec(db, sql, print_combobox, 0, &err_msg); // Execute la requête sql et envoie le résultat à la fonction "print_combobox"
    gtk_widget_show(dialog);

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

//  EXPORT\



//  /IMPORT

/*
 * Importation d'un calendrier :
 * Lis les valeurs d'un fichier texte provenant d'un autre calendrier pour les sauvegarder dans la base de donnée
 */
int import(void *NotUsed, int rowCount, char **rowValue, char **rowName){
    char line[500];
    while ( fgets(line, 500, save_file) != NULL) { // Tant que la ligne lu dans le fichier n'est pas nulle
        char * sql = build_sql(
                        build_sql(
                            build_sql(
                                build_sql(
                                    build_sql(sql,
                                    "INSERT INTO Day(content, date, calendar_id) VALUES("),
                                line),
                            ","),
                        rowValue[0]),
                 ")");
        sqlite3_exec(db, sql, NULL , 0, &err_msg); // Execute la requête sql
    }
    fclose(save_file);
    refresh_menu();
    return 0;
}

void    on_import_activate(){
    dialog = GTK_WIDGET(gtk_builder_get_object(builder, "dialog_import"));
    gtk_window_set_default_size(GTK_WINDOW(dialog), 400, 300);
    gtk_widget_show(dialog);
}

void    on_btn_validate_import_clicked(){
    entry = GTK_WIDGET(gtk_builder_get_object(builder, "entry_name_import"));
    char *getEntry = gtk_editable_get_chars(GTK_EDITABLE(entry), 0, -1);
    if (strcmp(getEntry_username, "") == 0) return;
    gtk_editable_delete_text(GTK_EDITABLE(entry), 0, -1);
    fileChooser = GTK_WIDGET(gtk_builder_get_object(builder, "fileChooser_import"));
    char *getFileName = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(fileChooser));
    if (strcmp(getFileName, "") != 0) {
        sprintf(str_id, "%d", user_id);
        char *sql = build_sql(
                build_sql(build_sql(build_sql("INSERT INTO Calendar(name, user_id) VALUES('", getEntry), "','"),
                          str_id), "')");
        sqlite3_exec(db, sql, NULL , 0, &err_msg); // Execute la requête sql

        save_file = fopen(getFileName, "rt");  // Ouvre le fichier texte en mode lecture

        sql = build_sql(build_sql("SELECT id FROM Calendar WHERE name='", getEntry), "'");
        sqlite3_exec(db, sql, import , 0, &err_msg); // Execute la requête sql
    }
    on_btn_cancel_clicked();
}

//  IMPORT\



//  /BTN CALENDAR

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

//  BTN CALENDAR\



//  /CREATE
void    on_new_activate(){
    dialog = GTK_WIDGET(gtk_builder_get_object(builder, "dialog_new"));
    gtk_window_set_default_size(GTK_WINDOW(dialog), 400, 300);
    gtk_widget_show(dialog);
}

void    on_btn_create_clicked(){
    if (user_id >= 0){
        entry = GTK_WIDGET(gtk_builder_get_object(builder, "entry_new_name"));
        char *getEntry = gtk_editable_get_chars(GTK_EDITABLE(entry), 0, -1);
        if (strcmp(getEntry, "") != 0) {
            sprintf(str_id, "%d", user_id);
            char *sql = build_sql(
                    build_sql(build_sql(build_sql("INSERT INTO Calendar(name, user_id) VALUES('", getEntry), "','"),
                              str_id), "')");

            sqlite3_exec(db, sql, NULL, 0,
                         &err_msg); // Execute la requête sql et envoie le résultat à la fonction "refresh_menu"
            refresh_menu();

            gtk_editable_delete_text(GTK_EDITABLE(entry), 0, -1);
            on_btn_cancel_clicked();
        }
    }
}

//  CREATE\



//  BACK TO MENU

void    on_btn_menu_calendar_clicked(){
    gtk_widget_hide(window);
    main_menu();
}



//  CALENDAR TIME
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