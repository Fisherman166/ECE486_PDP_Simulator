// place call back for buttons here for easy access and debug

#include "callback.h"
#include "gui.h"
#include "memory.h"
#include "main.h"
#include <errno.h>

extern uint8_t tracepoint_reached;	//From memory.h
extern uint8_t tracepoint_number;	//From memory.h

/********************************************************************
                             Buttons
********************************************************************/

/******************************************************************************
** CHOSES THE PAGE OF MEMORY VALUES TO PRINT OUT
******************************************************************************/
void spin_clicked (GtkSpinButton *spinbutton,
                   gpointer       user_data)
{
   int memory_page = gtk_spin_button_get_value_as_int (spinbutton);
	print_memory_page(memory_page);
   loadscreen((g_items*)user_data);
}


/******************************************************************************
** LOADS MEMORY PAGE DATA FROM TEXT FILE TO BUFFER
******************************************************************************/
void loadscreen( g_items *obj)
{
    gchar *contents;
    gsize length;

    obj->FP = g_file_new_for_path ("MEMORY_PAGE.txt");

    if (g_file_load_contents (obj->FP, NULL, &contents, &length, NULL, NULL))
    {
        gtk_text_buffer_set_text (obj->buffer, contents, length);
        g_free (contents);
    }
    g_free (obj->fname);

}

/******************************************************************************
** CLOSES THE SIMULATOR AND PRINTS STATS
******************************************************************************/
void exit_button_click(GtkButton *button, gpointer   data)
{
  g_items* local_object = (g_items*)data;
  shutdown_system(local_object->coherance_vars);
  gtk_widget_destroy(GTK_WIDGET(local_object->window));
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

	//Don't do anything if execution is complete
	if(local_object->coherance_vars->execution_done) goto EXECUTION_DONE;

	//Tell the GUI we have ran at least once and that we intend to run, not step
	local_object->coherance_vars->ran_once = 1;
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
	loadscreen_trace(local_object);	//update the trace window
	update_labels(local_object);

	if(local_object->coherance_vars->breakpoint_reached) {
		local_object->coherance_vars->breakpoint_reached = 0;

		local_object->msgbuff = gtk_text_view_get_buffer (GTK_TEXT_VIEW (local_object->messages_txt));
	 	sprintf(buffer_text, "Breakpoint reached at address %04o", 
					local_object->coherance_vars->registers_ptr->PC);
      gtk_text_buffer_set_text (local_object->msgbuff, buffer_text,-1);
	}

	if(tracepoint_reached) {
		tracepoint_reached = 0;

		local_object->msgbuff = gtk_text_view_get_buffer (GTK_TEXT_VIEW (local_object->messages_txt));
	 	sprintf(buffer_text, "Tracepoint at address %04o touched by instruction at address %04o",
					tracepoint_number,
					local_object->coherance_vars->registers_ptr->PC - 1);
      gtk_text_buffer_set_text (local_object->msgbuff, buffer_text, -1);
	}
	
EXECUTION_DONE:
	if(local_object->coherance_vars->execution_done) {
		local_object->msgbuff = gtk_text_view_get_buffer (GTK_TEXT_VIEW (local_object->messages_txt));
	 	sprintf(buffer_text, "Execution of program complete");
		gtk_text_buffer_set_text (local_object->msgbuff, buffer_text, -1);
		update_labels(local_object);
	}
}

/******************************************************************************
** EXECUTES ONE OPCODE BEFORE HALTING AGAIN
******************************************************************************/
void step_button_click(GtkButton *button, gpointer   data)
{
   g_items* local_object = (g_items*)data;
	char buffer_text[60];

	//Tell the GUI we have ran at least once and that we intend to step
	local_object->coherance_vars->ran_once = 1;
	local_object->coherance_vars->step_or_run = STEP;

	//Don't do anything if execution is complete
	if(local_object->coherance_vars->execution_done) goto EXECUTION_DONE;

	execute_opcode(local_object->coherance_vars);
	loadscreen_trace(local_object);	//update the trace window
	update_labels(local_object);

EXECUTION_DONE:
	if(local_object->coherance_vars->execution_done) {
		local_object->msgbuff = gtk_text_view_get_buffer (GTK_TEXT_VIEW (local_object->messages_txt));
	 	sprintf(buffer_text, "Execution of program complete");
		gtk_text_buffer_set_text (local_object->msgbuff, buffer_text, -1);
	}
}

/******************************************************************************
** PRINT OUT THE BRANCH TRACE FILE TO TEXT WINDOW
******************************************************************************/
void loadscreen_trace(g_items* local_object) {
	gchar *contents;
   gsize length;
	
	if(local_object->coherance_vars->branch_trace) {
		update_branch_trace();
		local_object->FP = g_file_new_for_path ("branch_trace.txt");
	}
	else {
		update_memory_trace();
		local_object->FP = g_file_new_for_path ("memory_trace.txt");
	}

	if (g_file_load_contents (local_object->FP, NULL, &contents, &length, NULL, NULL))
   {
   	gtk_text_buffer_set_text (local_object->trace_text_buffer, contents, length);
      g_free (contents); 
   }
   g_free (local_object->fname); 
}

/******************************************************************* *
                             Radio Buttons
********************************************************************/
void Display_Mem_trace (GtkWidget *button, gpointer   user_data)
{
    g_items* local_object = (g_items *) user_data;

    if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (button)))
    {
		  local_object->coherance_vars->branch_trace = 0;
		  loadscreen_trace(local_object);
		  #ifdef GUI_DEBUG
        g_print ("\nDisplay MEM Trace!\n");
		  #endif
    }

}

void Diplay_branch_trace(GtkWidget *button, gpointer   user_data)
{
    g_items* local_object = (g_items *) user_data;

 	if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (button))) {
		local_object->coherance_vars->branch_trace = 1;
		loadscreen_trace(local_object);
		#ifdef GUI_DEBUG
     	g_print ("\nDisplay Branch Trace!\n");
		#endif
  	}
}

/********************************************************************
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
   temp_ptr = (g_items *) user_data;

   int breakpoint_address = strtol(breakpoint_text, NULL, 8);	//Use octal base

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
** OUTPUTS THE MEMORY VALUE AT A SPECIFIED LOCATION IN THE MESSAGE BOX
******************************************************************************/
void print_memory_location (GtkEntry *entry, gpointer  user_data)
{
    const int maximum_address = 07777;	//Maximum address allowed in PDP8
    const char *memory_text;
    char buffer_text[100];
    g_items * local_object = (g_items *) user_data; 
    int memory_address;

    // gets the entry
    memory_text = gtk_entry_get_text(entry);
    memory_address = strtol(memory_text, NULL, 8);	//Use octal base
    // clear after getting the value 
    gtk_entry_set_text (entry,"\0");

     // get the buffer to be used
    local_object->msgbuff = gtk_text_view_get_buffer (GTK_TEXT_VIEW (local_object->messages_txt));
    if( memory_address > maximum_address)
    {
	 	sprintf(buffer_text, "Memory address %04o is not valid", memory_address);
      gtk_text_buffer_set_text (local_object->msgbuff, buffer_text, -1);
    }
    else
    {
	     sprintf(buffer_text, "The memory value at address %04o is: %04o", memory_address, (memory[memory_address] & MEMORY_MASK));
        gtk_text_buffer_set_text (local_object->msgbuff, buffer_text,-1);
    }
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

