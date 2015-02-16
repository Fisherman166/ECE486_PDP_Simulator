

typedef struct
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



/*
	gtk_text_view_set_editable (GtkTextView *text_view,
                            gboolean setting);
*/




