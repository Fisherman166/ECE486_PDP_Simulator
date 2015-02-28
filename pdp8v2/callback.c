// place call back for buttons here for easy access and debug

#include "callback.h"
#include "gui.h"
#include "memory.h"
#include "main.h"

extern uint8_t tracepoint_reached;	//From memory.h
extern uint8_t tracepoint_number;	//From memory.h

/********************************************************************
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

    obj->FP = g_file_new_for_path ("test1.txt");

    if (g_file_load_contents (obj->FP, NULL, &contents, &length, NULL, NULL))
    {
        gtk_text_buffer_set_text (obj->buffer, contents, length);
        g_free (contents);
    }
    g_free (obj->fname);

// c``reading a txt to test msg txt box
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

/******************************************************************************
** RUNS UNTIL A BREAKPOINT OR TRACEPOINT IS HIT
******************************************************************************/
void run_button_click (GtkButton *button,
                       gpointer   data)
{
	int thread1_return, thread2_return;
	pthread_t keyboard_thread, simulator_thread;
        g_items* local_object = (g_items*)data;
	char buffer_text[60];

	local_object->coherance_vars->step_or_run = RUN;
	 
	thread1_return = pthread_create(&keyboard_thread, NULL, read_keyboard, (void*)(local_object->coherance_vars->kb_ptr));
	if(thread1_return) {
		fprintf(stderr, "Keyboard thread failed\n");
		exit(-1);
   }

	thread2_return = pthread_create( &simulator_thread, NULL, run_program, (void*)(local_object->coherance_vars));
	if(thread2_return) {
		fprintf(stderr, "Simulator thread failed\n");
		exit(-2);
   }

	//Run the threads
	pthread_join(keyboard_thread, NULL);
	pthread_join(simulator_thread, NULL);

	//Check after thread exits
	if(local_object->coherance_vars->breakpoint_reached) {
		local_object->coherance_vars->breakpoint_reached = 0;
		update_labels(local_object);

		local_object->msgbuff = gtk_text_view_get_buffer (GTK_TEXT_VIEW (local_object->messages_txt));
	 	sprintf(buffer_text, "Breakpoint reached at address %04o", 
					local_object->coherance_vars->registers_ptr->PC);
      gtk_text_buffer_set_text (local_object->msgbuff, buffer_text,-1);
	}

	if(tracepoint_reached) {
		tracepoint_reached = 0;
		update_labels(local_object);

		local_object->msgbuff = gtk_text_view_get_buffer (GTK_TEXT_VIEW (local_object->messages_txt));
	 	sprintf(buffer_text, "Tracepoint at address %04o touched by instruction at address %04o",
					tracepoint_number,
					local_object->coherance_vars->registers_ptr->PC - 1);
      gtk_text_buffer_set_text (local_object->msgbuff, buffer_text, -1);
	}
	
	if(local_object->coherance_vars->execution_done) {
		local_object->msgbuff = gtk_text_view_get_buffer (GTK_TEXT_VIEW (local_object->messages_txt));
	 	sprintf(buffer_text, "Execution of program complete");
		gtk_text_buffer_set_text (local_object->msgbuff, buffer_text, -1);
		shutdown_system(local_object->coherance_vars);	
	}
}

/******************************************************************************
** EXECUTES ONE OPCODE BEFORE HALTING AGAIN
******************************************************************************/
void step_button_click(GtkButton *button, gpointer   data)
{
   g_items* local_object = (g_items*)data;
	local_object->coherance_vars->step_or_run = STEP;
	char buffer_text[60];

	execute_opcode(local_object->coherance_vars);
	update_labels(local_object);

	if(local_object->coherance_vars->execution_done) {
		local_object->msgbuff = gtk_text_view_get_buffer (GTK_TEXT_VIEW (local_object->messages_txt));
	 	sprintf(buffer_text, "Execution of program complete");
		gtk_text_buffer_set_text (local_object->msgbuff, buffer_text, -1);
		shutdown_system(local_object->coherance_vars);	
	}
}

/******************************************************************* *
                             Radio Buttons
********************************************************************/
// Note: Must likely we are not going to use them but I thought
// Seeing the function iside the if was useful mostlike to be used
// in conjunction with trace and breakpoint input
/*
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
*/

/******************************************************************* *
                             Text Boxes
********************************************************************/

/******************************************************************************
** DETERMINES IF WE WANT TO ADD OR REMOVE A BREAKPOINT
******************************************************************************/
void breakpoint_handler(GtkEntry *entry,
                    gpointer  user_data)
{
    g_items * temp_ptr;
    const char *breakpoint_text;
    breakpoint_text = gtk_entry_get_text(entry);
    int breakpoint_address = strtol(breakpoint_text, NULL, 8);	//Use octal base

    temp_ptr = (g_items *) user_data;


   if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (temp_ptr->radio_set_BP)))
   {
		breakpoint_to_set(breakpoint_address, temp_ptr);
	}
 	else{
		breakpoint_to_remove(breakpoint_address, temp_ptr);
   }

// clear screen to confirm entry
    gtk_entry_set_text (entry,"\0");
}

/******************************************************************************
** THIS FUNCTION ADDS A BREAKPOINT TO MEMORY
******************************************************************************/
void breakpoint_to_set(int breakpoint_address, g_items * obj)
{
    const int maximum_address = 07777;	//Maximum address allowed in PDP8
	 char buffer_text[100];

    obj->msgbuff = gtk_text_view_get_buffer (GTK_TEXT_VIEW (obj->messages_txt));
    if(breakpoint_address > maximum_address)
    {
	 	sprintf(buffer_text, "Breakpoint address %o is not valid", breakpoint_address);
      gtk_text_buffer_set_text (obj->msgbuff, buffer_text,-1);
    }
    else
    {
        set_breakpoint( (uint16_t)(breakpoint_address & 0xFFFF) );
	 	  sprintf(buffer_text, "Breakpoint set at address %04o", breakpoint_address);
        gtk_text_buffer_set_text (obj->msgbuff, buffer_text,-1);
    }
}

/******************************************************************************
** THIS FUNCTION REMOVES A BREAKPOINT FROM MEMORY
******************************************************************************/
void breakpoint_to_remove(int breakpoint_address, g_items * obj)
{
    const int maximum_address = 07777;	//Maximum address allowed in PDP8
	 char buffer_text[100];

    obj->msgbuff = gtk_text_view_get_buffer (GTK_TEXT_VIEW (obj->messages_txt));
    if(breakpoint_address > maximum_address)
    {
	 	sprintf(buffer_text, "Not a valid address");
      gtk_text_buffer_set_text (obj->msgbuff, buffer_text, -1);
    }
    else
    {
        remove_breakpoint( (uint16_t)(breakpoint_address & 0xFFFF) );
	 	  sprintf(buffer_text, "Breakpoint removed at address %04o", breakpoint_address);
        gtk_text_buffer_set_text (obj->msgbuff, buffer_text, -1);
    }
}

/******************************************************************************
** DETERMINES IF WE WANT TO ADD OR REMOVE A TRACEPOINT
******************************************************************************/
void tracepoint_handler(GtkEntry *entry,
                    gpointer  data)
{
   const char *tracepoint_text;
   g_items* local_object = (g_items*)data;
   tracepoint_text = gtk_entry_get_text(entry);
   int tracepoint_address = strtol(tracepoint_text, NULL, 8);	//Use octal base

	if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (local_object->radioB_SetTr))) {
		tracepoint_to_set(tracepoint_address, local_object);
   }
 	else {
    	tracepoint_to_remove(tracepoint_address, local_object); 
   }

	// clear screen to confirm entry
   gtk_entry_set_text (entry,"\0");
}

/******************************************************************************
** THIS FUNCTION ADDS A TRACEPOINT TO MEMORY
******************************************************************************/
void tracepoint_to_set(int tracepoint_address, g_items * obj)
{
    const int maximum_address = 07777;	//Maximum address allowed in PDP8
	 char buffer_text[100];

    obj->msgbuff = gtk_text_view_get_buffer (GTK_TEXT_VIEW (obj->messages_txt));
    if(tracepoint_address > maximum_address)
    {
	 	sprintf(buffer_text, "Tracepoint address %o is not valid", tracepoint_address);
      gtk_text_buffer_set_text (obj->msgbuff, buffer_text, -1);
    }
    else
    {
        set_tracepoint( (uint16_t)(tracepoint_address & 0xFFFF) );
	 	  sprintf(buffer_text, "Tracepoint set at address %04o", tracepoint_address);
        gtk_text_buffer_set_text (obj->msgbuff, buffer_text, -1);
    }
}

/******************************************************************************
** THIS FUNCTION REMOVES A TRACEPOINT FROM MEMORY
******************************************************************************/
void tracepoint_to_remove(int tracepoint_address, g_items * obj)
{
    const int maximum_address = 07777;	//Maximum address allowed in PDP8
	 char buffer_text[100];

    obj->msgbuff = gtk_text_view_get_buffer (GTK_TEXT_VIEW (obj->messages_txt));
    if(tracepoint_address > maximum_address)
    {
	 	sprintf(buffer_text, "Not a valid address");
      gtk_text_buffer_set_text (obj->msgbuff, buffer_text, -1);
    }
    else
    {
        remove_tracepoint( (uint16_t)(tracepoint_address & 0xFFFF) );
	 	  sprintf(buffer_text, "Tracepoint removed at address %04o", tracepoint_address);
        gtk_text_buffer_set_text (obj->msgbuff, buffer_text, -1);
    }
}

