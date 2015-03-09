/******************************************************************************
** ECE486/586 PDP-8 Simulator
** Sean Koppenhafer, Luis Santiago, Ken Benderly, J.S. Peirce
** 
** callback.c - Implements the callback functions for the buttons, radio buttons,
** and text boxes found in the GUI.
******************************************************************************/

#include "callback.h"
#include "gui.h"
#include "memory.h"
#include "main.h"

extern uint8_t tracepoint_reached;	//From memory.h
extern uint8_t tracepoint_number;	//From memory.h

/********************************************************************
                             Buttons
********************************************************************/

/******************************************************************************
** CHOSES THE PAGE OF MEMORY VALUES TO PRINT OUT
******************************************************************************/
void page_number_entry_callback (GtkEntry *entry, gpointer  user_data)
{
   g_items * local_object;
   const char *pagenumber_text;
	const char *octal_error = "The inputted address is not a valid octal address";
	static int valid_octal;
   pagenumber_text = gtk_entry_get_text(entry);
   local_object = (g_items *) user_data;

	check_if_octal( atoi(pagenumber_text), &valid_octal );

	local_object->msgbuff = gtk_text_view_get_buffer (GTK_TEXT_VIEW (local_object->messages_txt));
	if(valid_octal) {
   	local_object->coherance_vars->last_mem_page = strtol(pagenumber_text, NULL, 8);	//Use octal base
      print_memory_page(local_object->coherance_vars->last_mem_page);
		update_memory_buffer(local_object);
      gtk_text_buffer_set_text (local_object->msgbuff, "", -1);
	}
	else {
      gtk_text_buffer_set_text (local_object->msgbuff, octal_error, -1);
	}

   // clear screen to confirm entry
    gtk_entry_set_text (entry,"\0");
}

/******************************************************************************
** LOADS A BUFFER ON THE GUI WITH TEXT FROM A TEXTFILE
******************************************************************************/
void load_buffer(g_items *object, const char* text_name, GtkTextBuffer* text_buffer)
{
    gchar *contents;
    gsize length;

    object->FP = g_file_new_for_path (text_name);

    if (g_file_load_contents (object->FP, NULL, &contents, &length, NULL, NULL))
    {
        gtk_text_buffer_set_text (text_buffer, contents, length);
        g_free (contents);
    }
    g_free (object->fname);
}

/******************************************************************************
** SHOW ALL BREAKPOINTS
******************************************************************************/
void show_breakpoints_click (GtkButton *button, gpointer   data)
{
	g_items* local_object = (g_items*)data;

	print_breakpoints();
	load_buffer(local_object, "breakpoints.txt", local_object->msgbuff);
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
	//Update the screen values
	loadscreen_trace(local_object);	//update the trace window
	update_labels(local_object);

	//Update the text files and the buffer
	print_memory_page(local_object->coherance_vars->last_mem_page);
	print_memory_list();
	update_memory_buffer(local_object);

	if(local_object->coherance_vars->breakpoint_reached) {
		local_object->coherance_vars->breakpoint_reached = 0;
		local_object->coherance_vars->breakpoint_serviced = 1;

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

	//Update the screen values
	loadscreen_trace(local_object);	//update the trace window
	update_labels(local_object);

	//Update text files and buffer
	print_memory_page(local_object->coherance_vars->last_mem_page);
	print_memory_list();
	update_memory_buffer(local_object);

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

/******************************************************************************
** PRINTS THE BRANCH TRACE FILE TO THE GUI WINDOW
******************************************************************************/
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

/******************************************************************************
** PRINTS THE BRANCH TRACE FILE TO THE GUI WINDOW
******************************************************************************/
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

/******************************************************************************
** PRINTS THE LIST FILE TO THE SCREEN
******************************************************************************/
void display_lst_file(GtkWidget *button, gpointer   user_data)
{
   g_items* local_object = (g_items *) user_data;

 	if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (button))) {
        load_buffer(local_object, "MEMORY_LIST.txt", local_object->buffer);
  	}
}

/******************************************************************************
** PRINTS THE PAGE NUMBER 
******************************************************************************/
void display_memory_page(GtkWidget *button, gpointer   user_data)
{
   g_items* local_object = (g_items *) user_data;

 	if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (button))) {
        load_buffer(local_object, "MEMORY_PAGE.txt", local_object->buffer);
  	}
}

/******************************************************************************
** CHOOSES TO UPDATE THE MEMORY PAGE FILE OR THE LIST FILE
******************************************************************************/
void update_memory_buffer(g_items* local_object) {
	const char* page_file = "MEMORY_PAGE.txt";
	const char* list_file = "MEMORY_LIST.txt";

 	if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (local_object->display_lst_file))) {
   	load_buffer(local_object, list_file, local_object->buffer);
	}
	else {
   	load_buffer(local_object, page_file, local_object->buffer);
	}
}

/******************************************************************************
** DETERMINES IF WE WANT TO ADD OR REMOVE A BREAKPOINT
******************************************************************************/
void breakpoint_handler(GtkEntry *entry, gpointer  user_data)
{
   g_items * local_object;
   const char *breakpoint_text;
	const char *octal_error = "The inputted address is not a valid octal address";
	static int valid_octal;
   int breakpoint_address; 
   breakpoint_text = gtk_entry_get_text(entry);
   local_object = (g_items *) user_data;

	check_if_octal( atoi(breakpoint_text), &valid_octal );

	if(valid_octal) {
   	breakpoint_address = strtol(breakpoint_text, NULL, 8);	//Use octal base

		if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (local_object->radio_set_BP)))
		{
			breakpoint_to_set(breakpoint_address, local_object);
		}
		else{
			breakpoint_to_remove(breakpoint_address, local_object);
		}
	}
	else {
		local_object->msgbuff = gtk_text_view_get_buffer (GTK_TEXT_VIEW (local_object->messages_txt));
      gtk_text_buffer_set_text (local_object->msgbuff, octal_error, -1);
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
	 const char *octal_error = "The inputted address is not a valid octal address";
    char buffer_text[100];
    g_items * local_object = (g_items *) user_data; 
    int memory_address;
	 static int valid_octal;
    memory_text = gtk_entry_get_text(entry);
	
	 check_if_octal( atoi(memory_text), &valid_octal );

	 local_object->msgbuff = gtk_text_view_get_buffer (GTK_TEXT_VIEW (local_object->messages_txt));
	 if(valid_octal) {
		 memory_address = strtol(memory_text, NULL, 8);	//Use octal base
		 if( memory_address > maximum_address)
		 {
			sprintf(buffer_text, "Memory address %04o is out of range", memory_address);
			gtk_text_buffer_set_text (local_object->msgbuff, buffer_text, -1);
		 }
		 else
		 {
			  sprintf(buffer_text, "The memory value at address %04o is: %04o", memory_address, (memory[memory_address] & MEMORY_MASK));
			  gtk_text_buffer_set_text (local_object->msgbuff, buffer_text,-1);
		 }
	}
	else {
      gtk_text_buffer_set_text (local_object->msgbuff, octal_error, -1);
	}
	gtk_entry_set_text (entry,"\0");
}

/******************************************************************************
** THIS FUNCTION GETS SR VALUE FROM GUI
******************************************************************************/
void sr_entry_callback (GtkEntry *entry, gpointer  user_data)
{
   const int maximum_address = 07777;	//Maximum address allowed in PDP8
   const char *sr_text;
   char buffer_text[100];
   g_items *local_object = (g_items *) user_data; 
   int sr_value;
	static int valid_octal;
   sr_text = gtk_entry_get_text(entry);

	check_if_octal( atoi(sr_text), &valid_octal);
	local_object->msgbuff = gtk_text_view_get_buffer (GTK_TEXT_VIEW (local_object->messages_txt));
	if(valid_octal) {
		sr_value = strtol(sr_text, NULL, 8);	//Use octal base
		 
	 	if(sr_value > maximum_address) {
			sprintf(buffer_text, "SR value larger than 12 bits");
		}
		else {
			local_object->coherance_vars->registers_ptr->SR = sr_value;
		 	sprintf(buffer_text, "SR value entered:  %04o", sr_value);
		}
	}
	else {
	 	sprintf(buffer_text, "Not an octal value");
	}

	gtk_text_buffer_set_text (local_object->msgbuff, buffer_text,-1);
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
	 	sprintf(buffer_text, "Breakpoint address %o is out of range", breakpoint_address);
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
	 	sprintf(buffer_text, "Breakpoint address %04o is out of range", breakpoint_address);
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
	const char *octal_error = "The inputted address is not a valid octal address";
	static int valid_octal;
   g_items* local_object = (g_items*)data;
   tracepoint_text = gtk_entry_get_text(entry);

	check_if_octal( atoi(tracepoint_text), &valid_octal );

	if(valid_octal) {
		int tracepoint_address = strtol(tracepoint_text, NULL, 8);	//Use octal base

		if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (local_object->radioB_SetTr))) {
			tracepoint_to_set(tracepoint_address, local_object);
		}
		else {
			tracepoint_to_remove(tracepoint_address, local_object); 
		}
	}
	else {
		local_object->msgbuff = gtk_text_view_get_buffer (GTK_TEXT_VIEW (local_object->messages_txt));
      gtk_text_buffer_set_text (local_object->msgbuff, octal_error, -1);
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

/******************************************************************************
** CHECKS TO SEE IF THE INPUTTED VALUE FROM THE USER IS VALID OCTAL VALUE
** RETURNS 1 IF INPUT IS A VALID OCTAL VALUE
** RETURNS 0 IF INPUT IS NOT A VALID OCTAL VALUE
******************************************************************************/
void check_if_octal(int input, int* value_good) {
	int tens_place = input / 10;	//Checks if number is less than 10
	int mod_value = input % 10;

 	if ( (tens_place == 0) && (mod_value < 8) ) { // less than 10 and valid octal 
    	*value_good = 1;
   }
  	else if(mod_value > 7) { //Not a valid octal value
    	*value_good = 0;
   }
   else if (tens_place > 0) {
   	check_if_octal(tens_place, value_good);
  	}
}

