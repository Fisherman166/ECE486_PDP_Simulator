// gui.h

#ifndef _GUI_H
#define _GUI_H

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct g_tag
{
    GtkWidget *grid;
    GtkEntryBuffer * breakpoints;
    GtkWidget *radio_BP1, *radio_BP2,*radioB_SetTr, *radioB_ClrTr;
    GtkWidget *button1, * button2, *button3, *button4;
    GtkWidget *pagenum, *label, *PC_label, *Accumulator_label, *CPMA_label, *Mesages_label;
    GtkWidget *value, *PC_value, *Accumulator_value, *CPMA_value;
    GtkWidget *scrolled_window, * scrolled_msg;
    GtkTextBuffer *buffer, *msgbuff;
    GtkWidget *text_view,*BreakP_entry,*Trace_entry, *messages_txt;
    GtkWidget *spin_button;
    GtkAdjustment *adjustment;
    GtkTextMark * mark;
    GtkTextIter *i;
    
    int c;

	GFile *FP;
	gchar *fname;
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

#endif
