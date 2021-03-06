// gui.h

#ifndef _GUI_H
#define _GUI_H

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct g_tag
{
    GtkWidget *grid;
    GtkWidget  * run_button, *step;
// name of all label not to be changed change next group
    GtkWidget *pagenum, *label, *PC_label, *Accumulator_label, *CPMA_label, *Mesages_label, *IR_label, *Link_label, *Memory_Buffer_label;

// name if labels to be changed with the values of the registers
    GtkWidget *value, *PC_value, *Accumulator_value, *CPMA_value, *IR_value, *Link_value, *Memory_Buffer_value;


// windows, buffers  buttons and misc stuff
    GtkWidget *scrolled_window, * scrolled_msg;
    GtkEntryBuffer * breakpoints;
    GtkTextBuffer *buffer, *msgbuff;
    GtkWidget *text_view, *BreakP_entry,*Trace_entry, *messages_txt;
    GtkWidget *spin_button;
    GtkWidget *radio_set_BP, *radio_clear_BP,*radioB_SetTr, *radioB_ClrTr;
    GtkAdjustment *adjustment;  
    //GtkTextMark * mark;    unsuccesfully try to use to change txt in mesage box
    //GtkTextIter *iter;

// file descripto for screen    
 	GFile *FP;
	gchar *fname;
    int c;


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
void entry_box_cb(g_items *obj);
void radio_button_callbacks( g_items * obj);
void create_buttons_callbacks(g_items* obj);
void activate (GtkApplication *app, gpointer    data);
int stall_execution;
#endif
