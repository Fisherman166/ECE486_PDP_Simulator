  // place call back for buttons here for easy access and debug
#ifndef _CALLBACK_H_
#define _CALLBACK_H_

//#include <gtk/gtk.h>
#include "gui.h"


/******************************************************************* *
                            PROTOTYPES Buttons
********************************************************************/

void spin_clicked (GtkSpinButton *spinbutton, gpointer user_data);
void loadscreen(g_items* obj);
void run_button_click (GtkButton *button1, gpointer   data);
void set_breakpt_cb(GtkWidget *button, gpointer   user_data);
void clear_breakpt_cb(GtkWidget *button, gpointer   user_data);
void set_tracepoint (GtkWidget *button, gpointer   user_data);
void clear_tracepoint(GtkWidget *button, gpointer   user_data);
void new_breakpoint(GtkEntry *entry, gpointer  user_data);
void new_tracepoint(GtkEntry *entry, gpointer  user_data);
void breakpoint_to_set(int breakpoint_address, g_items * obj);
void step_button_click(GtkButton *button, gpointer   data);

#endif



