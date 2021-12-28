#include <stdio.h>
#include <gtk/gtk.h>

GtkApplication *app;
GtkWidget *window;
GtkBuilder *builder;
GtkWidget *saveButton;
GtkWidget *textview;
GtkTextBuffer *textbuffer;
GtkWidget *day1;

char tmp[1024]; //Variable for textview


int main(int argc, char **argv){

    gtk_init(&argc, &argv);

    builder = gtk_builder_new_from_file("InterfaceM28.glade");

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));

    app = gtk_application_new ("test.gtk.calendar", G_APPLICATION_FLAGS_NONE);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);


    day1 = GTK_WIDGET(gtk_builder_get_object(builder, "day"));

    saveButton = GTK_WIDGET(gtk_builder_get_object(builder, "saveButton"));
    textview = GTK_WIDGET(gtk_builder_get_object(builder, "textview"));


    gtk_builder_connect_signals(builder, NULL);

    gtk_widget_show(window);

    gtk_main();

    return 0;
}

void on_day_clicked (GtkButton *b){
    //printf("Test");

    gtk_widget_hide(window);
    builder = gtk_builder_new_from_file("textEditW.glade");

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));


    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_builder_connect_signals(builder, NULL);

    saveButton = GTK_WIDGET(gtk_builder_get_object(builder, "saveButton"));
    textview = GTK_WIDGET(gtk_builder_get_object(builder, "textview"));
    //TextEdit
    textbuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(textview));
    strcpy(tmp, "test");
    gtk_text_buffer_set_text(textbuffer, (const gchar *) tmp, (gint) -1);


    gtk_widget_show(window);

}

void on_saveButton_clicked (GtkButton *b){
    //printf("Hello");
    GtkTextIter begin, end;
    gchar * text;
    gtk_text_buffer_get_iter_at_offset(GTK_TEXT_BUFFER(textbuffer), &begin, (gint) 0);
    gtk_text_buffer_get_iter_at_offset(GTK_TEXT_BUFFER(textbuffer), &end, (gint) -1);

    text = gtk_text_buffer_get_text(GTK_TEXT_BUFFER(textbuffer), &begin, &end, TRUE);
    //printf("%s", text);
}