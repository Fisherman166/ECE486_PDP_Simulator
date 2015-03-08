/******************************************************************************
** ECE486/586 PDP-8 Simulator
** Sean Koppenhafer, Luis Santiago, Ken Benderly, J.S. Peirce
** 
** callback.h
******************************************************************************/

#ifndef _CALLBACK_H_
#define _CALLBACK_H_

#include "gui.h"

/******************************************************************* *
                            PROTOTYPES Buttons
********************************************************************/
void load_buffer(g_items*, const char*, GtkTextBuffer*);

void page_number_entry_callback (GtkEntry *entry, gpointer  user_data);
void run_button_click (GtkButton *button1, gpointer   data);
void step_button_click(GtkButton *button, gpointer   data);
void exit_button_click(GtkButton *button, gpointer   data);
void show_breakpoints_click(GtkButton *button, gpointer   data);
void print_memory_location (GtkEntry *entry, gpointer  user_data);
void loadscreen_trace(g_items*);

//Tracepoint and breakpoint functions
void breakpoint_handler(GtkEntry *entry, gpointer  user_data);
void tracepoint_handler(GtkEntry *entry, gpointer  user_data);
void breakpoint_to_set(int breakpoint_address, g_items * obj);
void breakpoint_to_remove(int breakpoint_address, g_items * obj);
void tracepoint_to_set(int, g_items*);
void tracepoint_to_remove(int, g_items*);
void sr_entry_callback (GtkEntry *entry, gpointer  user_data);

//Intermediate functions
void Diplay_branch_trace(GtkWidget *button, gpointer   user_data);
void Display_Mem_trace (GtkWidget *button, gpointer   user_data);
void check_if_octal(int, int*);

#endif



