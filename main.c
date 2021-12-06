#include <stdio.h>
#include <gtk/gtk.h>


int main(int argc, char **argv) {
    GtkWidget *fenetre_principale = NULL;
    GtkBuilder *builder = NULL;
    GError *error = NULL;
    gchar *filename = NULL;
    /* Initialisation de la librairie Gtk. */
    gtk_init(&argc, &argv);

    printf("Hello, World!\n");
    return 0;
}