// projgui.c

#include "gui.h"
#include "callback.h"

// compile with  
// gcc projgui.c -o proj $(pkg-config --cflags --libs gtk+-3.0)


int main (int argc, char **argv)
{
    GtkApplication *app;
    int status;
    g_items *obj = malloc(sizeof(*obj));

    app = gtk_application_new ("ECE.Project", G_APPLICATION_FLAGS_NONE);
    g_signal_connect (app, "activate", G_CALLBACK (activate), obj);
    status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);

    free(obj);
    return status;
}

