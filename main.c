#include <stdio.h>
#include <gtk/gtk.h>

GtkApplication *app;
GtkWidget *window;
GtkBuilder *builder;
GtkWidget *fixed1;
GtkWidget *day1;
GtkWidget *label1;


int main(int argc, char **argv){

    gtk_init(&argc, &argv);

    builder = gtk_builder_new_from_file("InterfaceM.glade");

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));

    app = gtk_application_new ("test.gtk.calendar", G_APPLICATION_FLAGS_NONE);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    fixed1 = GTK_WIDGET(gtk_builder_get_object(builder, "fixed1"));
    day1 = GTK_WIDGET(gtk_builder_get_object(builder, "day"));
    label1 = GTK_WIDGET(gtk_builder_get_object(builder, "label1"));


    gtk_builder_connect_signals(builder, NULL);

    gtk_widget_show(window);

    gtk_main();

    return 0;
}

void on_day1_clicked (GtkButton *b){
    //printf("Test");

    gtk_widget_hide(window);
    builder = gtk_builder_new_from_file("Test.glade");

    window = GTK_WIDGET(gtk_builder_get_object(builder, "window"));

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_builder_connect_signals(builder, NULL);

    gtk_widget_show(window);
}