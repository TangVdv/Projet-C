#include <stdio.h>
#include <gtk/gtk.h>

GtkApplication *app;
GtkWidget *window;
GtkBuilder *builder;
GtkWidget *fixed1;
GtkWidget *button1;
GtkWidget *label1;


int main(int argc, char **argv){

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