#include <stdio.h>
#include <gtk/gtk.h>

GtkApplication *app;
GtkWidget *window;
GtkBuilder *builder;
GtkWidget *saveButton;
GtkWidget *textview;
GtkWidget *btn_calendar_day;
GtkTextBuffer *textbuffer;
const gchar *btn_id;
const gchar *btn_value = "";

char tmp[1024]; //Variable for textview

void on_day_clicked (GtkWidget *b);

void show_calendar(){
    builder = gtk_builder_new_from_file("InterfaceM30_BIS.glade");

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    gtk_window_set_default_size(GTK_WINDOW(window), 700, 700);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *grid = GTK_WIDGET(gtk_builder_get_object(builder, "grid_calendar"));
    char *btn_name;
    char charValue = '0';
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 7; ++j) {
            btn_name = (char *) malloc(10 *sizeof(char));

            sprintf(btn_name, "%d", i);
            strncat(btn_name, "_", 10);
            charValue = j+'0';
            strncat(btn_name, &charValue, 10);

            btn_calendar_day = gtk_button_new_with_label(btn_value);
            gtk_widget_set_hexpand(btn_calendar_day, TRUE);
            gtk_widget_set_vexpand(btn_calendar_day, TRUE);
            gtk_widget_set_name(btn_calendar_day, btn_name);
            g_signal_connect(G_OBJECT(btn_calendar_day), "clicked", G_CALLBACK(on_day_clicked), NULL);

            gtk_grid_attach(GTK_GRID(grid), btn_calendar_day, i,j, 1, 1);

            free(btn_name);
        }
    }
    gtk_builder_connect_signals(builder, NULL);

    gtk_widget_show_all(window);
}

void show_text_edit(){
    builder = gtk_builder_new_from_file("textEditW.glade");

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

int main(int argc, char **argv){

    gtk_init(&argc, &argv);

    show_calendar();

    gtk_main();

    return 0;
}

void on_day_clicked (GtkWidget *b){
    //printf("Test");
    gtk_widget_hide(window);
    btn_id = gtk_widget_get_name(GTK_WIDGET(b));
    btn_value = gtk_button_get_label(GTK_BUTTON(b));

    show_text_edit();
}

void on_save_button_clicked (GtkButton *b){
    gtk_widget_hide(window);
    //printf("Hello");
    GtkTextIter begin, end;

    gtk_text_buffer_get_iter_at_offset(GTK_TEXT_BUFFER(textbuffer), &begin, (gint) 0);
    gtk_text_buffer_get_iter_at_offset(GTK_TEXT_BUFFER(textbuffer), &end, (gint) -1);

    btn_value = gtk_text_buffer_get_text(GTK_TEXT_BUFFER(textbuffer), &begin, &end, TRUE);

    show_calendar();
}