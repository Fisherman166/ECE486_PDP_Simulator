  // place call back for buttons here for easy access and debug

#include "gui.h"




/******************************************************************* *
                             Buttons
********************************************************************/

static void
spin_clicked (GtkSpinButton *spinbutton,
              gpointer       user_data)
{
 int value = gtk_spin_button_get_value_as_int (spinbutton);

}

void loadscreen(g_items* obj)
{
gchar *contents;
char * data1;
  gsize length;
//obj->i=1;

 obj->FP = g_file_new_for_path ("add01.as");
// obj->fname = g_file_get_basename (obj->FP);

 if (g_file_load_contents (obj->FP, NULL, &contents, &length, NULL, NULL))
{
  gtk_text_buffer_set_text (obj->buffer, contents, length);
 g_free (contents);
}
g_free (obj->fname);

// c``reating a txt to test msg txt box
  data1="my txt\n";
  contents = (gchar*) data1; 
  printf( " my msg: %s ", data1);
  g_print ("\nGprint: %s", contents);


 
// Prolems loaing txt to 2nd screen without using file system 
// as above

//    obj->mark = gtk_text_buffer_get_insert (obj->msgbuff);
 //    gtk_text_buffer_get_iter_at_mark (buffer, &obj->iter, obj->mark);
 //    gtk_text_buffer_insert (buffer, &iter, text, -1);

//obj->mark = gtk_text_buffer_get_mark (contents,TRUE);
// gtk_text_buffer_get_iter_at_mark (obj->buffer, &obj->iter, obj->mark);


//gtk_text_buffer_insert (obj->msgbuff,obj->mark, contents, sizeof(contents));
  //gtk_text_buffer_insert(contents, contents, sizeof(contents));
// obj-> = gtk_text_buffer_new ("new txt");   //input buffer
  
     }
   

void on_button_click (GtkButton *button1,
                             gpointer   data)
{
    g_items * tptr;
    char buff[4];
    tptr = (g_items *) data;
    ++(tptr->c);
// store int into buffer
    sprintf(buff, "%d", tptr->c);
    gtk_label_set_text(GTK_LABEL (tptr->PC_value),buff);

   loadscreen(tptr);
}

/******************************************************************* *
                             Radio Buttons
********************************************************************/
// Note: Must likely we are not going to use them but I thought 
// Seeing the function iside the if was useful mostlike to be used 
// in conjunction with trace and breakpoint input 

void set_breakpt_cb(GtkWidget *button,
                   gpointer   user_data)
{
	if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (button)))
	{
      	    g_print ("\nSET BKP!\n");
	}

}


void clear_breakpt_cb(GtkWidget *button,
                   gpointer   user_data)
{
	if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (button)))
	{
      	    g_print ("\nClear BKP!\n");
	}
}


void set_tracepoint (GtkWidget *button,
                   gpointer   user_data)
{
	if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (button)))
	{
      	    g_print ("\nSET tracepoint!\n");
	}

}


void clear_tracepoint(GtkWidget *button, gpointer   user_data)
{
	if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (button)))
	{
      	    g_print ("\nClear Tracepoint!\n");
	}
}


/******************************************************************* *
                             Text Boxes
********************************************************************/

void new_breakpoint(GtkEntry *entry,
             gpointer  user_data)
{
  const char *val;
  val = gtk_entry_get_text (entry);
 
  g_print ("\nBreakpoint entered %d!\n\n", atoi(val));
  gtk_entry_set_text (entry,"\0");

}


void new_tracepoint(GtkEntry *entry,
             gpointer  user_data)
{
  const char *val;
  val = gtk_entry_get_text (entry);
 
  g_print ("\nTrace point entered %d!\n\n", atoi(val));
  gtk_entry_set_text (entry,"\0");

}




