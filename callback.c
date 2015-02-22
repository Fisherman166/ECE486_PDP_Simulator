  // place call back for buttons here for easy access and debug

#include "callback.h"
#include "gui.h"
#include "memory.h"
#include "main.h"

/******************************************************************* *
                             Buttons
********************************************************************/

void spin_clicked (GtkSpinButton *spinbutton,
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

 obj->FP = g_file_new_for_path ("test1.txt");
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
 //obj-> = gtk_text_buffer_new ("new txt");   //input buffer
  
     }
   

void run_button_click (GtkButton *button,
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
  g_items * temp_ptr;
  const char *breakpoint_text;
  breakpoint_text = gtk_entry_get_text(entry);
  int breakpoint_address = strtol(breakpoint_text, NULL, 8);	//Use octal base
 
   temp_ptr = (g_items *) user_data;


  if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON ((GtkWidget *)temp_ptr->radio_set_BP)))
   {
      	  breakpoint_to_set(breakpoint_address, temp_ptr);
	}

 
// clear screen to confiem entry
  gtk_entry_set_text (entry,"\0");
}




 void breakpoint_to_set(int breakpoint_address, g_items * obj)
 {
   const int maximum_address = 07777;	//Maximum address allowed in PDP8

  if(breakpoint_address > maximum_address) {
  		#ifdef DEBUG_GUI
  		g_print("Breakpoint address %04o is not valid\n", breakpoint_address);
  		#endif
    
  }
  else {
  		set_breakpoint( (uint16_t)(breakpoint_address & 0xFFFF) );
  		g_print ("Breakpoint entered at %04o!\n", breakpoint_address);
     
       gtk_text_buffer_get_iter_at_line (obj->msgbuff,iter,1);

  obj->msgbuff = gtk_text_view_get_buffer (GTK_TEXT_VIEW (obj->messages_txt));
      gtk_text_buffer_set_text (obj->msgbuff, "Hello, this is some text\0", sizeof("Hello, this is some text"));
  }


}





void new_tracepoint(GtkEntry *entry,
             gpointer  user_data)
{
  const char *val;
  val = gtk_entry_get_text (entry);
 
  g_print ("\nTrace point entered %d!\n\n", atoi(val));
  gtk_entry_set_text (entry,"\0");

}

