// gui.h

#ifndef _GUI_H
#define _GUI_H

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"

typedef struct g_tag
{   
    GtkWidget *window;
    GtkWidget *grid;
 // top buttons
    GtkWidget  * run_button, *step, *show_breakpoints, * exit_button;
// name of all labels
    GtkWidget *pagenum, *label, *PC_label, *Accumulator_label, *CPMA_label;
    GtkWidget *Mesages_label, *IR_label, *Link_label, *Memory_Buffer_label;
    GtkWidget *memory_entry_label, *SR_label;    

// name if labels to be changed with the values of the registers
    GtkWidget *value, *PC_value, *Accumulator_value, *CPMA_value;
    GtkWidget *IR_value, *Link_value, *Memory_Buffer_value;

// windows, buffers
    GtkWidget *scrolled_window, * scrolled_msg, *text_view, *messages_txt;
    GtkWidget *trace_text_window, *trace_text_view;

// buffers
    GtkTextBuffer *buffer, *msgbuff, *trace_text_buffer;
    
// Breakpoint and Trace windows
    GtkWidget  *BreakP_entry,*Trace_entry;
    GtkWidget  *memory_entry, *SR_entry, *page_number_entry;;



// Buttons 
    GtkWidget *radio_set_BP, *radio_clear_BP,*radioB_SetTr, *radioB_ClrTr;   // radio buttons
    GtkWidget * memory_pages, *memory_trace, *branch_trace;


  
// file descripto for screen
    GFile *FP;
    gchar *fname;

	 //Local stuff
    int copy_argc;
    char ** copy_argv;
    struct shared_vars* coherance_vars; 
} g_items;


// PROTOTYPES
void create_labels(g_items * obj);
void create_buttons(g_items* obj);
void create_entrybox (g_items* obj);
void set_grid(g_items * obj);
void create_buffers(g_items * obj);
void create_text_boxes (g_items * obj);
void create_radio_buttons(g_items* obj);
void set_radio_buttons( g_items *obj);
//void entry_box_cb(g_items *obj);
void radio_button_callbacks( g_items * obj);
void create_buttons_callbacks(g_items* obj);
void activate (GtkApplication *app, gpointer    data);
void update_labels(g_items*);
#endif
