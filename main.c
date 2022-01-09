#include <stdio.h>
#include <gtk/gtk.h>
#include <sqlite3.h>
#include <string.h>
#include <time.h>
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
GtkWidget *btn_calendar_day;
GtkWidget *grid;
GtkWidget *textview;
GtkWidget *saveButton;
GtkTextBuffer *textbuffer;
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
GtkWidget *btn_id;
const gchar *btn_value = "";
const char *btn_name;

char tmp[1024]; //Variable for textview


char *getEntry_username, *getEntry_password;

int user_id = -1;
char str[10];
int user_type = -1;
int year, month_id, day;
int current_year, current_month,  current_day;
char month_name[12][10] = {"Janvier","Février","Mars","Avril","Mai","Juin","Juillet","Août","Septembre\0","Octobre","Novembre","Décembre"};

void    on_btn_delete_clicked(GtkWidget *b);
void    on_btn_overview_clicked(GtkWidget *b);
void    on_day_clicked (GtkWidget *click_button);


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

    return 0;
}

void refresh_menu(){
    GList *children, *iter;
    // TODO : Understand and comment this shit
    children = gtk_container_get_children(GTK_CONTAINER(calendar_frame));
    for (iter = children; iter != NULL; iter = g_list_next(iter))
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    g_list_free(children);

    if (user_id >= 0) {
        sprintf(str, "%d", user_id);
        char *sql = build_sql("SELECT * FROM Calendar WHERE user_id = ", str); // Création d'une requête sql
        sqlite3_exec(db, sql, refresh_menu_callback, 0,
                     &err_msg); // Execute la requête sql et envoie le résultat à la fonction "refresh_menu_callback"

        gtk_widget_show_all(window);

        gtk_widget_set_visible(compteMenuItem, FALSE);
        gtk_widget_set_visible(fichierMenuItem, TRUE);
    }
    else {
        gtk_widget_show_all(window);
        gtk_widget_set_visible(fichierMenuItem, FALSE);
        gtk_widget_set_visible(compteMenuItem, TRUE);
    }
}

void main_menu(){

    builder = gtk_builder_new_from_file("Test.glade");

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    gtk_window_set_default_size(GTK_WINDOW(window), 1000, 500);
    gtk_widget_show_all(window);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_builder_connect_signals(builder, NULL);

    calendar_frame = GTK_WIDGET(gtk_builder_get_object(builder, "Calendar_btn_grid"));
    ListStore = GTK_LIST_STORE(gtk_builder_get_object(builder, "ListStore"));
    fichierMenuItem = GTK_WIDGET(gtk_builder_get_object(builder, "fichier"));
    compteMenuItem = GTK_WIDGET(gtk_builder_get_object(builder, "compte"));
    gtk_widget_show_all(window);

    refresh_menu();

}

//  MAIN MENU\



//  /CALENDAR

/*
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
        printf("%c", buffer[i]);
        if (buffer[i] == '\n') {
            linenumber++;
            printf("%d:\t", linenumber);
        }

    }
    printf("\n");
    return bytes;
}
*/

int print_label_calendar(void *NotUsed, int rowCount, char **rowValue, char **rowName){
    GtkWidget *label_calendar_name = GTK_WIDGET(gtk_builder_get_object(builder, "label_calendar_name"));
    GtkWidget *label_current_date = GTK_WIDGET(gtk_builder_get_object(builder, "label_current_date"));
    gtk_label_set_text(GTK_LABEL(label_calendar_name), rowValue[0]);

    char *current_date = malloc(50);
    strcpy(current_date,"Date du jour : ");
    sprintf(str, "%d", current_day);
    strcat(current_date, str);
    strcat(current_date, " ");
    strcat(current_date, month_name[current_month]);
    strcat(current_date, " ");
    sprintf(str, "%d", current_year);
    strcat(current_date, str);

    gtk_label_set_text(GTK_LABEL(label_current_date), current_date);
    gtk_widget_show_all(window);
}

int set_label_button(void *NotUsed, int rowCount, char **rowValue, char **rowName){
    gtk_button_set_label(GTK_BUTTON(btn_calendar_day), rowValue[0]);
}

void refresh_calendar(){


    // SELECT name FROM Calendar WHERE id = btn_calendar_id
    char * sql = build_sql("SELECT name FROM Calendar WHERE id=",btn_calendar_id);
    sqlite3_exec(db, sql, print_label_calendar, 0, &err_msg); // Execute la requête sql

    GtkWidget *label_year = GTK_WIDGET(gtk_builder_get_object(builder, "label_year"));
    GtkWidget *label_month = GTK_WIDGET(gtk_builder_get_object(builder, "label_month"));

    sprintf(str,"%d",year);
    gtk_label_set_text(GTK_LABEL(label_year), str);
    gtk_label_set_text(GTK_LABEL(label_month), month_name[month_id]);


    grid = GTK_WIDGET(gtk_builder_get_object(builder, "grid_day"));

    GList *children, *iter;
    // TODO : Understand and comment this shit
    children = gtk_container_get_children(GTK_CONTAINER(grid));
    for (iter = children; iter != NULL; iter = g_list_next(iter))
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    g_list_free(children);

    int dayMax;

    if(month_id == 1){
        if(year%4==0 && year%100!=0 || year%400==0) dayMax = 29;
        else dayMax = 28;
    }
    else {
        if (month_id == 3 || month_id == 5 || month_id == 8 || month_id == 10)
            dayMax = 30;
        else
            dayMax = 31;
    }

    day = 0;
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 7; ++j) {
            day += 1;
            char *btn_name = (char *) malloc(11);

            sprintf(btn_name, "%d", year);
            strcat(btn_name, "-");
            sprintf(str, "%d",month_id+1);
            strcat(btn_name, str);
            strcat(btn_name, "-");
            sprintf(str, "%d", day);
            strcat(btn_name, str);


            GtkWidget *box_btn_day = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

            GtkWidget *label_calendar_day = gtk_label_new(str);
            btn_calendar_day = gtk_button_new();
            gtk_widget_set_hexpand(btn_calendar_day, TRUE);
            gtk_widget_set_vexpand(btn_calendar_day, TRUE);
            gtk_widget_set_name(btn_calendar_day, btn_name);
            g_signal_connect(G_OBJECT(btn_calendar_day), "clicked", G_CALLBACK(on_day_clicked), NULL);

            gtk_box_pack_start(GTK_BOX(box_btn_day),label_calendar_day,0,0,0);
            gtk_box_pack_start(GTK_BOX(box_btn_day),btn_calendar_day,1,1,0);

            gtk_grid_attach(GTK_GRID(grid), box_btn_day, j,i, 1, 1);

            char *sql = build_sql(build_sql(build_sql("SELECT content FROM Day WHERE date='",btn_name),"' AND calendar_id="),btn_calendar_id);
            sqlite3_exec(db, sql, set_label_button, 0, &err_msg); // Execute la requête sql

            free(btn_name);

            if(day == dayMax) break;
        }
        if(day == dayMax) break;
    }

    gtk_widget_show_all(window);
}

void main_calendar() {

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

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_builder_connect_signals(builder, NULL);

    if (btn_id != NULL) {
        /*
        const char *btn_name = gtk_widget_get_name(btn_id);
        gtk_button_set_label(day, "test");
        printf("print\n");
        //printf("btn_name = %s\n", btn_name);
        //btn_calendar_day = gtk_button_new_with_label(btn_value);
        printf("btn_label : %s\n", gtk_button_get_label(btn_id));
        printf("btn_value : %s\n", btn_value);

        //gtk_button_set_label(GTK_BUTTON(btn_id), textTest);
         */
    }
    refresh_calendar();
}

//  CALENDAR\


//  /DAY

int is_day_exist(void *NotUsed, int rowCount, char **rowValue, char **rowName){
    char *sql;
    //printf("btn_value : %s\n", btn_value);
    //printf("btn_name : %s\n", btn_name);
    //printf("btn_calendar_id : %s\n", btn_calendar_id);
    if(rowValue[1] == NULL) {
        printf("\n\nInsert\n\n");
        //INSERT INTO Day(content, date, calendar_id) VALUES(btn_value, btn_name, btn_calendar_id)
        sql = build_sql(build_sql(build_sql(build_sql(
                build_sql(build_sql("INSERT INTO Day(content, date, calendar_id) VALUES('", btn_value), "','"),
                btn_name), "',"), btn_calendar_id), ")");
        sqlite3_exec(db, sql, NULL, 0, &err_msg); // Execute la requête sql
    }
    else{
        if(strcmp(btn_value, "") == 0){
            printf("\n\nDelete\n\n");
            //DELETE FROM Day WHERE id = rowValue[1]
            sql = build_sql("DELETE FROM Day WHERE id=",rowValue[1]);
            sqlite3_exec(db, sql, NULL, 0, &err_msg); // Execute la requête sql
        }
        else {
            printf("\n\nUpdate\n\n");

            //UPDATE Day SET content = btn_value WHERE id = rowValue[1]
            sql = build_sql(build_sql(build_sql("UPDATE Day SET content='", btn_value), "' WHERE id="), rowValue[1]);
            printf("sql : %s\n", sql);
            sqlite3_exec(db, sql, NULL, 0, &err_msg);
        }
    }

    main_calendar();
}

void show_text_edit(){
    gtk_widget_hide(window);

    builder = gtk_builder_new_from_file("textEdit.glade");

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 500);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_builder_connect_signals(builder, NULL);

    saveButton = GTK_WIDGET(gtk_builder_get_object(builder, "saveButton"));
    textview = GTK_WIDGET(gtk_builder_get_object(builder, "textview"));
    //TextEdit
    textbuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
    strcpy(tmp, btn_value);
    gtk_text_buffer_set_text(textbuffer, (const gchar *) tmp, (gint) -1);

    gtk_widget_show_all(window);
}

void on_day_clicked (GtkWidget *click_button){
    gtk_widget_hide(window);
    btn_id = click_button;

    if ( gtk_button_get_label(GTK_BUTTON(click_button)) != NULL)
        btn_value = gtk_button_get_label(GTK_BUTTON(click_button));
    else
        btn_value = "";

    printf("btn_value : %s\n", btn_value);
    show_text_edit();
}

void on_save_button_clicked (GtkButton *b){
    gtk_widget_hide(window);
    GtkTextIter begin, end;

    gtk_text_buffer_get_iter_at_offset(GTK_TEXT_BUFFER(textbuffer), &begin, (gint) 0);
    gtk_text_buffer_get_iter_at_offset(GTK_TEXT_BUFFER(textbuffer), &end, (gint) -1);

    btn_value = gtk_text_buffer_get_text(GTK_TEXT_BUFFER(textbuffer), &begin, &end, TRUE);
    btn_name = gtk_widget_get_name(btn_id);

    //SELECT id FROM Day WHERE date = btn_name AND calendar_id = btn_calendar_id
    char *sql = build_sql(build_sql(build_sql("SELECT COUNT(), id FROM Day WHERE date='",btn_name),"' AND calendar_id="),btn_calendar_id);
    sqlite3_exec(db, sql, is_day_exist, 0, &err_msg);

}

//  DAY\



// MAIN
int main(int argc, char **argv){
    printf("Start\n\n");

    // `time_t` is an arithmetic time type
    time_t now;

    // Obtain current time
    // `time()` returns the current time of the system as a `time_t` value
    time(&now);

    // localtime converts a `time_t` value to calendar time and
    // returns a pointer to a `tm` structure with its members
    // filled with the corresponding values
    struct tm *local = localtime(&now);

    current_day = local->tm_mday;            // get day of month (1 to 31)
    current_month = local->tm_mon + 1;      // get month of year (0 to 11)
    current_month -= 1;
    month_id = current_month;
    current_year = local->tm_year + 1900;   // get year since 1900
    year = current_year;

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


//  CANCEL
void    on_btn_cancel_clicked(){
    gtk_widget_hide(dialog);
}



// EXIT

void    on_exit_activate(){
    gtk_main_quit();
}



// LOGOUT

void    on_logout_activate(){
    user_id = -1;
    user_type = -1;
    refresh_menu();
}


// HASH PASSWORD

char *hash_pwd(char *password){
    size_t size = strlen(password);
    char *value = malloc(size + 1);
    char *hash_password = malloc(255);
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
    char *sql = build_sql(
            build_sql(
                    build_sql(
                            build_sql(
                                    "SELECT id,name,type FROM Account GROUP BY id HAVING COUNT(id) = 1 AND name='", getEntry_username),
                            "' AND password='"),
                    getEntry_password),
            "'");

    sqlite3_exec(db, sql, check_login_exist_callback, 0,&err_msg); // Execute la requête sql et envoie le résultat à la fonction "refresh_menu"
    on_btn_cancel_clicked();
}

void    on_login_activate(){
    dialog = GTK_WIDGET(gtk_builder_get_object(builder, "dialog_login"));
    gtk_window_set_default_size(GTK_WINDOW(dialog), 400, 300);
    gtk_widget_show(dialog);
}

void    on_btn_login_clicked(){
    entry = GTK_WIDGET(gtk_builder_get_object(builder, "entry_name_login"));
    getEntry_username = gtk_editable_get_chars(GTK_EDITABLE(entry), 0, -1);
    if (strcmp(getEntry_username, "") == 0) return;
    gtk_editable_delete_text(GTK_EDITABLE(entry), 0, -1);
    entry = GTK_WIDGET(gtk_builder_get_object(builder, "entry_password_login"));
    getEntry_password = gtk_editable_get_chars(GTK_EDITABLE(entry), 0, -1);
    if (strcmp(getEntry_password, "") == 0) return;
    gtk_editable_delete_text(GTK_EDITABLE(entry), 0, -1);

    getEntry_password = hash_pwd(getEntry_password);

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
                                        "INSERT INTO Account(name, password) VALUES('", getEntry_username),
                                "','"),
                        getEntry_password),
                "')");
        sqlite3_exec(db, sql, NULL, 0,&err_msg); // Execute la requête sql et envoie le résultat à la fonction "refresh_menu"
        login();
    }
    else {
        printf("user '%s' already exist\n", getEntry_username);
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

int print_combobox(void *NotUsed, int rowCount, char **rowValue, char **rowName){
    gtk_list_store_append(ListStore, &iter);
    gtk_list_store_set(ListStore, &iter, 0, rowValue[0], -1);
}

/*
 * Exportation d'un calendrier :
 * Récupère les valeurs dans la base de donnée pour les insérer dans un fichier texte
 */
int export_callback(void *NotUsed, int rowCount, char **rowValue, char **rowName) {
    for (int i = 0; i < rowCount; i++) {
        printf("%s = %s\n", rowName[i], rowValue[i]);
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
    sprintf(str, "%d", user_id);
    char *sql = build_sql(build_sql("SELECT name FROM Calendar WHERE user_id='", str),"'"); // Création d'une requête sql
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

        // SELECT * FROM Day INNER JOIN Calendar ON Day.calendar_id = Calendar.id WHERE Calendar.name = getEntry
        char *sql = build_sql(build_sql("SELECT content, date FROM Day INNER JOIN Calendar ON Day.calendar_id = Calendar.id WHERE Calendar.name = '", getEntry),"'"); // Création d'une requête sql
        printf("sql : %s\n",sql);
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
        //INSERT INTO Day(content, date, calendar_id) VALUES(line, rowValue[0])
        printf("id : %s\n", rowValue[0]);
        char *sql = "";
        sql = build_sql(build_sql(build_sql(build_sql(build_sql(sql,"INSERT INTO Day(content, date, calendar_id) VALUES("),line),","),rowValue[0]),")");
        printf("sql : %s\n\n", sql);
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
        sprintf(str, "%d", user_id);
        //INSERT INTO Calendar(name, user_id) VALUES(getEntry, str)
        char *sql = build_sql(build_sql(build_sql(build_sql("INSERT INTO Calendar(name, user_id) VALUES('", getEntry), "','"),str), "')");
        sqlite3_exec(db, sql, NULL , 0, &err_msg); // Execute la requête sql

        save_file = fopen(getFileName, "rt");  // Ouvre le fichier texte en mode lecture

        //SELECT id FROM Calendar WHERE name = getEntry
        sql = build_sql(build_sql("SELECT id FROM Calendar WHERE name='", getEntry), "'");
        sqlite3_exec(db, sql, import , 0, &err_msg); // Execute la requête sql
    }
    on_btn_cancel_clicked();
}

//  IMPORT\



//  /BTN CALENDAR

void    on_btn_overview_clicked(GtkWidget *b){
    gtk_widget_hide(window);

    btn_calendar_id = gtk_widget_get_name(b);

    main_calendar();

}

void    on_btn_delete_clicked(GtkWidget *b){
    btn_calendar_id =   gtk_widget_get_name(b);
    char * sql = build_sql("DELETE FROM Calendar WHERE id=", btn_calendar_id);
    sqlite3_exec(db, sql, NULL, 0, &err_msg); // Execute la requête sql
    sql = build_sql("DELETE FROM Day WHERE calendar_id=",btn_calendar_id);
    sqlite3_exec(db, sql, NULL, 0, &err_msg); // Execute la requête sql
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
            sprintf(str, "%d", user_id);
            char *sql = build_sql(
                    build_sql(build_sql(build_sql("INSERT INTO Calendar(name, user_id) VALUES('", getEntry), "','"),
                              str), "')");

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