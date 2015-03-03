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
void step_button_click(GtkButton *button, gpointer   data);
void exit_button_click(GtkButton *button, gpointer   data);
void print_memory_location (GtkEntry *entry, gpointer  user_data);

//Tracepoint and breakpoint functions
void breakpoint_handler(GtkEntry *entry, gpointer  user_data);
void tracepoint_handler(GtkEntry *entry, gpointer  user_data);
void breakpoint_to_set(int breakpoint_address, g_items * obj);
void breakpoint_to_remove(int breakpoint_address, g_items * obj);
void tracepoint_to_set(int, g_items*);
void tracepoint_to_remove(int, g_items*);

void Diplay_branch_trace(GtkWidget *button, gpointer   user_data);
void Display_Mem_trace (GtkWidget *button, gpointer   user_data);

#endif



