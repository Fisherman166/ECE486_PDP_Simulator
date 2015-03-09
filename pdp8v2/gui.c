/******************************************************************************
** ECE486/586 PDP-8 Simulator
** Sean Koppenhafer, Luis Santiago, Ken Benderly, J.S. Peirce
** 
** gui.c - Implements functions that add content to the GUI window
******************************************************************************/

#include "gui.h"
#include "callback.h"
#include "main.h"

/*Create a label and set its alignment. Setting the line wrap to TRUE makes
the label break lines if the text exceeds the widget's size. When set to
FALSE the text gets cut off by the edge of the widget*/
void create_labels(g_items * obj)
{
    obj->pagenum= gtk_label_new ("Select a Page Number");
    obj->label = gtk_label_new ("Register");
    obj->PC_label= gtk_label_new ("PC");
    obj->Accumulator_label= gtk_label_new ("AC");
    obj->CPMA_label= gtk_label_new ("CPMA");
    obj->IR_label= gtk_label_new ("IR");
    obj->Link_label= gtk_label_new ("Link");
    obj->Memory_Buffer_label= gtk_label_new ("Memory Buffer");
    obj->memory_entry_label= gtk_label_new ("Memory Value At Address"); 
    obj->Mesages_label= gtk_label_new ("Messages");
    obj->SR_label=gtk_label_new ("SR"); 

    gtk_label_set_justify (GTK_LABEL (obj->pagenum), GTK_JUSTIFY_CENTER);
    gtk_label_set_justify (GTK_LABEL (obj->label), GTK_JUSTIFY_LEFT);
    gtk_label_set_justify (GTK_LABEL (obj->PC_label), GTK_JUSTIFY_LEFT);
    gtk_label_set_justify (GTK_LABEL (obj->Accumulator_label), GTK_JUSTIFY_LEFT);
    gtk_label_set_justify (GTK_LABEL (obj->CPMA_label), GTK_JUSTIFY_LEFT);
    gtk_label_set_justify (GTK_LABEL (obj->Mesages_label), GTK_JUSTIFY_CENTER);
    gtk_label_set_justify (GTK_LABEL (obj->Mesages_label), GTK_JUSTIFY_LEFT);
    gtk_label_set_justify (GTK_LABEL (obj->SR_label), GTK_JUSTIFY_LEFT);

// values

    obj->value = gtk_label_new ("Contents");
    obj->PC_value= gtk_label_new ("0");
    obj->Accumulator_value= gtk_label_new ("0");
    obj->CPMA_value = gtk_label_new ("0");
    obj->IR_value= gtk_label_new ("0");
    obj->Link_value= gtk_label_new ("0");
    obj->Memory_Buffer_value= gtk_label_new ("0");

    gtk_label_set_justify (GTK_LABEL (obj->value), GTK_JUSTIFY_LEFT);
    gtk_label_set_justify (GTK_LABEL (obj->PC_value), GTK_JUSTIFY_RIGHT);
    gtk_label_set_justify (GTK_LABEL (obj->Accumulator_value), GTK_JUSTIFY_LEFT);
    gtk_label_set_justify (GTK_LABEL (obj->CPMA_value), GTK_JUSTIFY_RIGHT);
}
/****************************************************************************
                       create buttons
****************************************************************************/
void create_buttons(g_items* obj)
{
// radio button needs to be created and label created


    obj->run_button = gtk_button_new_with_label ("RUN");
    obj->show_breakpoints = gtk_button_new_with_label ("Show Breakpoints");
    obj->exit_button = gtk_button_new_with_label ("Exit");
    obj->step = gtk_button_new_with_label ("Step");
}

/****************************************************************************
                      entry box
****************************************************************************/
void create_entrybox (g_items* obj)
{
    obj->BreakP_entry = gtk_entry_new();
    obj->Trace_entry = gtk_entry_new();
    obj->memory_entry= gtk_entry_new();
    obj->SR_entry= gtk_entry_new();
    obj->page_number_entry= gtk_entry_new();

}

/********************************************************************
  Create a grid and attach the button 
********************************************************************/
void set_grid(g_items * obj)
{
    int lcol ,c1row, c2row;
    lcol =3;
    c1row = c2row = 2;

    obj->grid = gtk_grid_new ();
    gtk_grid_set_column_spacing (GTK_GRID (obj->grid), 10);
    gtk_grid_set_row_spacing (GTK_GRID (obj->grid), 3);
    gtk_grid_set_column_homogeneous (GTK_GRID (obj->grid), TRUE);

    //**************************************************(left ,Top, width, height)
    gtk_widget_set_vexpand (obj->scrolled_window, TRUE);
    //gtk_grid_attach (GTK_GRID (obj->grid), obj->scrolled_window ,1, 4, 2, 5);

    /* set button location on the grid */
    gtk_grid_attach (GTK_GRID (obj->grid), obj->run_button ,1, 1, 1, 1);
    gtk_grid_attach_next_to (GTK_GRID (obj->grid),obj->step, obj->run_button,GTK_POS_RIGHT,1,1);
    gtk_grid_attach_next_to (GTK_GRID (obj->grid),obj->show_breakpoints, obj->step,GTK_POS_RIGHT,1,1);
    gtk_grid_attach_next_to (GTK_GRID (obj->grid),obj->exit_button, obj->show_breakpoints,GTK_POS_RIGHT,1,1);
    gtk_grid_attach_next_to (GTK_GRID (obj->grid),obj->pagenum, obj->step,GTK_POS_BOTTOM,1,1);
    gtk_grid_attach_next_to (GTK_GRID (obj->grid),obj->page_number_entry, obj->pagenum,GTK_POS_BOTTOM,1,1);
    // radio button at top of the screen 
    gtk_grid_attach_next_to (GTK_GRID (obj->grid),obj->display_memory_page, obj->page_number_entry,GTK_POS_BOTTOM,1,1);
    gtk_grid_attach_next_to (GTK_GRID (obj->grid),obj->display_lst_file, obj->display_memory_page ,GTK_POS_LEFT,1,1);
    // place top text screen
    gtk_grid_attach_next_to (GTK_GRID (obj->grid),obj->scrolled_window, obj->display_lst_file ,GTK_POS_BOTTOM,2,5);

    /****************************************************************************
    NOTE: variables lcol and c1row are uto incremented to ensure that
    adding new things to the grid will be the one next to ot the on e
    below it
    *****************************************************************************/

//***********************************************************(left ,Top, width, height)
    gtk_grid_attach (GTK_GRID (obj->grid), obj->label , lcol , c1row , 1, 1);
    gtk_grid_attach_next_to (GTK_GRID (obj->grid),obj->PC_label, obj->label,GTK_POS_BOTTOM,1,1);
    gtk_grid_attach_next_to (GTK_GRID (obj->grid),obj->Accumulator_label, obj->PC_label,GTK_POS_BOTTOM,1,1);
    gtk_grid_attach_next_to (GTK_GRID (obj->grid),obj->CPMA_label, obj->Accumulator_label,GTK_POS_BOTTOM,1,1);
    gtk_grid_attach_next_to (GTK_GRID (obj->grid),obj->IR_label, obj->CPMA_label,GTK_POS_BOTTOM,1,1);
    gtk_grid_attach_next_to (GTK_GRID (obj->grid),obj->Link_label, obj->IR_label,GTK_POS_BOTTOM,1,1);
    gtk_grid_attach_next_to (GTK_GRID (obj->grid),obj->Memory_Buffer_label, obj->Link_label,GTK_POS_BOTTOM,1,1);
    gtk_grid_attach_next_to (GTK_GRID (obj->grid),obj->SR_label, obj->Memory_Buffer_label,GTK_POS_BOTTOM,1,1);
    

//*IR_label *Link_label, Memory_Buffer_label

    gtk_grid_attach (GTK_GRID (obj->grid), obj->value    ,(++lcol), c2row , 1, 1);
    gtk_grid_attach (GTK_GRID (obj->grid), obj->PC_value, (lcol), ++c2row , 1, 1);
    gtk_grid_attach (GTK_GRID (obj->grid), obj->Accumulator_value ,lcol, ++c2row , 1, 1);
    gtk_grid_attach (GTK_GRID (obj->grid), obj->CPMA_value ,lcol, ++c2row, 1, 1);
    gtk_grid_attach_next_to (GTK_GRID (obj->grid),obj->IR_value, obj->CPMA_value,GTK_POS_BOTTOM,1,1);
    gtk_grid_attach_next_to (GTK_GRID (obj->grid),obj->Link_value, obj->IR_value,GTK_POS_BOTTOM,1,1);
    gtk_grid_attach_next_to (GTK_GRID (obj->grid),obj->Memory_Buffer_value, obj->Link_value,GTK_POS_BOTTOM,1,1);
    gtk_grid_attach_next_to (GTK_GRID (obj->grid),obj->SR_entry, obj->Memory_Buffer_value,GTK_POS_BOTTOM,1,1);

// radio Buttons placement
    gtk_grid_attach_next_to (GTK_GRID (obj->grid),obj->radio_set_BP, obj->SR_entry,GTK_POS_BOTTOM,1,1);
    gtk_grid_attach_next_to (GTK_GRID (obj->grid),obj->radio_clear_BP, obj->radio_set_BP,GTK_POS_BOTTOM,1,1);
    gtk_grid_attach_next_to (GTK_GRID (obj->grid),obj->BreakP_entry,obj->radio_clear_BP, GTK_POS_BOTTOM, 1,1);
    gtk_grid_attach_next_to (GTK_GRID (obj->grid),obj->memory_entry,obj->BreakP_entry, GTK_POS_BOTTOM, 1,1);
    
    gtk_grid_attach_next_to (GTK_GRID (obj->grid),obj->memory_entry_label, obj->memory_entry ,GTK_POS_LEFT,1,1);
    // set and clear trace points 
    gtk_grid_attach_next_to (GTK_GRID (obj->grid),obj->radioB_SetTr, obj->SR_label,GTK_POS_BOTTOM,1,1);
    gtk_grid_attach_next_to (GTK_GRID (obj->grid),obj->radioB_ClrTr, obj->radioB_SetTr,GTK_POS_BOTTOM,1,1);
    gtk_grid_attach_next_to (GTK_GRID (obj->grid),obj->Trace_entry, obj->radioB_ClrTr,GTK_POS_BOTTOM,1,1);

// buttons at the buttom of text window 
    gtk_grid_attach_next_to (GTK_GRID (obj->grid),obj->memory_trace,obj->scrolled_window ,GTK_POS_BOTTOM,1,1);
    gtk_grid_attach_next_to (GTK_GRID (obj->grid),obj->branch_trace,obj->memory_trace ,GTK_POS_RIGHT,1,1);
    gtk_grid_attach_next_to (GTK_GRID (obj->grid),obj->trace_text_window, obj->memory_trace,GTK_POS_BOTTOM,2,10); 

// message window
    gtk_grid_attach_next_to (GTK_GRID (obj->grid),obj->Mesages_label, obj->memory_entry_label ,GTK_POS_BOTTOM,2,1);
    gtk_grid_attach_next_to (GTK_GRID (obj->grid),obj->scrolled_msg, obj->Mesages_label,GTK_POS_BOTTOM,2,1);
}

void create_buffers(g_items * obj)
{
    /* text view windows with buffers
      does not add scroll functionality here
     that is done in create_test_boxes */

    /*****************************************************************
        Text view is a widget in which can display the text buffer.
        The line wrapping is set to break lines in between words.
    *******************************************************************/
    /* The text buffer represents the text being edited */
    obj->buffer = gtk_text_buffer_new (NULL);   //input buffer
    obj->text_view = gtk_text_view_new_with_buffer (obj->buffer);
    gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (obj->text_view), GTK_WRAP_WORD);

    obj->msgbuff = gtk_text_buffer_new (NULL);   //input buffer
    obj->messages_txt = gtk_text_view_new_with_buffer (obj->msgbuff);
    gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (obj->messages_txt), GTK_WRAP_WORD);

    obj->trace_text_buffer = gtk_text_buffer_new (NULL);   //input buffer
    obj->trace_text_view = gtk_text_view_new_with_buffer (obj->trace_text_buffer);
    gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (obj->trace_text_view), GTK_WRAP_WORD);
}

void create_text_boxes (g_items * obj)
{
    /* Create the scrolled window. Usually NULL is passed for both parameters so
     * that it creates the horizontal/vertical adjustments automatically. Setting
     * the scrollbar policy to automatic allows the scrollbars to only show up
     * when needed.
     */
    obj->scrolled_window = gtk_scrolled_window_new (NULL, NULL);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (obj->scrolled_window),
                                    GTK_POLICY_AUTOMATIC,
                                    GTK_POLICY_AUTOMATIC);

    /* The function directly below is used to add children to the scrolled window
     * with scrolling capabilities (e.g text_view), otherwise,
     * gtk_scrolled_window_add_with_viewport() would have been used
     */
    gtk_container_add (GTK_CONTAINER (obj->scrolled_window), obj->text_view);
    gtk_container_set_border_width (GTK_CONTAINER (obj->scrolled_window), 5);

// here sets messages box

    obj->scrolled_msg = gtk_scrolled_window_new (NULL, NULL);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (obj->scrolled_msg),
                                    GTK_POLICY_AUTOMATIC,
                                    GTK_POLICY_AUTOMATIC);

    gtk_container_add (GTK_CONTAINER (obj->scrolled_msg), obj->messages_txt);
    gtk_container_set_border_width (GTK_CONTAINER (obj->scrolled_msg), 5);


   // here set trace memory and branch trace memory text window
    obj->trace_text_window = gtk_scrolled_window_new (NULL, NULL);
    gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (obj->trace_text_window),
                                    GTK_POLICY_AUTOMATIC,
                                    GTK_POLICY_AUTOMATIC);

    gtk_container_add (GTK_CONTAINER (obj->trace_text_window), obj->trace_text_view);
    gtk_container_set_border_width (GTK_CONTAINER (obj->trace_text_window), 5);


}

/*****************************************************************
                     CREATE RADIO BUTTONS
*******************************************************************/
void create_radio_buttons(g_items* obj)
{
    obj->radio_set_BP=gtk_radio_button_new_with_label(NULL, "Set Breakpoint");

    obj->radio_clear_BP=gtk_radio_button_new_with_label_from_widget
                        (GTK_RADIO_BUTTON (obj->radio_set_BP), "Clear Breakpoint");

    obj->radioB_SetTr =gtk_radio_button_new_with_label(NULL, "Set Tracepoint");
    obj->radioB_ClrTr =gtk_radio_button_new_with_label_from_widget(
                       GTK_RADIO_BUTTON (obj->radioB_SetTr), "Clear Tracepoint");


    obj->memory_trace =gtk_radio_button_new_with_label(NULL, "Memory Trace");
    obj->branch_trace =gtk_radio_button_new_with_label_from_widget(
                       GTK_RADIO_BUTTON (obj->memory_trace), "Branch Trace");

    obj->display_memory_page = gtk_radio_button_new_with_label(NULL, "Display Memory Page");
    obj->display_lst_file = gtk_radio_button_new_with_label_from_widget(
                             GTK_RADIO_BUTTON (obj->display_memory_page), "Display List File");
}

/*****************************************************************
                      SET VALUES OF THE RADIO BUTTONS
*******************************************************************/
void set_radio_buttons( g_items *obj)
{
// set values
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (obj->radio_set_BP), TRUE);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (obj->radio_clear_BP), FALSE);

    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (obj->radioB_SetTr), TRUE);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (obj->radioB_ClrTr), FALSE);

  
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (obj->memory_trace), TRUE);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (obj->branch_trace), TRUE);

     gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (obj->display_lst_file), TRUE);
     gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (obj->display_memory_page), FALSE);   
}

/*****************************************************************
                     Create entry box callbacks
*******************************************************************/
void entry_box_cb(g_items *obj)
{
    g_signal_connect (GTK_ENTRY (obj->BreakP_entry), "activate",
                      G_CALLBACK (breakpoint_handler), obj);

    g_signal_connect (GTK_ENTRY (obj->Trace_entry), "activate",
                      G_CALLBACK (tracepoint_handler), obj);

     g_signal_connect (GTK_ENTRY (obj->memory_entry), "activate",
                      G_CALLBACK (print_memory_location), obj);
     
    g_signal_connect (GTK_ENTRY (obj->SR_entry), "activate",
                      G_CALLBACK (sr_entry_callback), obj);

 g_signal_connect (GTK_ENTRY (obj->page_number_entry), "activate",
                      G_CALLBACK (page_number_entry_callback), obj);
}

/*****************************************************************
                      RADIO BUTTONS CALLBACKS
*******************************************************************/
void radio_button_callbacks( g_items * obj)
{
  // trace points
   g_signal_connect (GTK_TOGGLE_BUTTON (obj->memory_trace), "toggled",
                      G_CALLBACK (Display_Mem_trace),  obj);

    g_signal_connect (GTK_TOGGLE_BUTTON (obj->branch_trace), "toggled",
                      G_CALLBACK (Diplay_branch_trace),  obj);

   g_signal_connect (GTK_TOGGLE_BUTTON (obj->display_lst_file), "toggled",
                      G_CALLBACK (display_lst_file),  obj);

    g_signal_connect (GTK_TOGGLE_BUTTON (obj->display_memory_page), "toggled",
                      G_CALLBACK (display_memory_page),  obj);
}

/*****************************************************************
                      CREATE BUTTONS CALL BACK
*******************************************************************/
void create_buttons_callbacks(g_items* obj)
{
    /*Connecting the clicked signal to the callback function*/

    g_signal_connect (GTK_BUTTON (obj->run_button), "clicked",
                      G_CALLBACK (run_button_click), obj);

    g_signal_connect (GTK_BUTTON (obj->step), "clicked",
                      G_CALLBACK (step_button_click), obj);

    g_signal_connect (GTK_BUTTON (obj->show_breakpoints), "clicked",
                      G_CALLBACK (show_breakpoints_click), obj);

    g_signal_connect (GTK_BUTTON (obj->exit_button), "clicked",
                      G_CALLBACK (exit_button_click), obj);
}

void activate (GtkApplication *app, gpointer    data)
{
    g_items * obj;
    obj = (g_items *) data;

    /******************************************************************
                        create window and set size
    ******************************************************************/
    obj->window = gtk_application_window_new (app);
    gtk_window_set_default_size (GTK_WINDOW (obj->window), 900, 800);
    gtk_window_set_title (GTK_WINDOW (obj->window), "ECE486/586 PDP8 Project");


// call funtion that makes labels

    create_labels(obj);
    create_buttons(obj);
    create_buffers(obj);
    create_radio_buttons( obj);
    set_radio_buttons(obj);
    create_text_boxes(obj);
    create_entrybox (obj);
    radio_button_callbacks(obj);
    create_buttons_callbacks(obj);
    entry_box_cb(obj);
    set_grid(obj);

	 //Init the system
	 obj->coherance_vars = malloc( sizeof(struct shared_vars) );
	 init_system(obj->copy_argc, obj->copy_argv, obj->coherance_vars);	//Initialize the simulator
	 //Start the memory page window at page 0
	 print_memory_page(0);
	 print_memory_list();
	 update_memory_buffer(obj);

    gtk_container_add (GTK_CONTAINER (obj->window), GTK_WIDGET (obj->grid));
    gtk_widget_show_all (obj->window);
}

void update_labels(g_items* object) {
	uint8_t buffer_size = 4;
	char PC_text[buffer_size];
	char AC_text[buffer_size];
	char CPMA_text[buffer_size];
	char IR_text[buffer_size];
	char Link_text[buffer_size];
	char MB_text[buffer_size];

	sprintf(PC_text, "%04o", object->coherance_vars->registers_ptr->PC);
	sprintf(AC_text, "%04o", object->coherance_vars->registers_ptr->AC);
	sprintf(CPMA_text, "%04o", object->coherance_vars->registers_ptr->CPMA);
	sprintf(IR_text, "%01o", object->coherance_vars->registers_ptr->IR);
	sprintf(Link_text, "%01o", object->coherance_vars->registers_ptr->link_bit);
	sprintf(MB_text, "%04o", object->coherance_vars->registers_ptr->MB);

   gtk_label_set_text(GTK_LABEL (object->PC_value), PC_text);
   gtk_label_set_text(GTK_LABEL (object->Accumulator_value), AC_text);
   gtk_label_set_text(GTK_LABEL (object->CPMA_value), CPMA_text);
   gtk_label_set_text(GTK_LABEL (object->IR_value), IR_text);
   gtk_label_set_text(GTK_LABEL (object->Link_value), Link_text);
   gtk_label_set_text(GTK_LABEL (object->Memory_Buffer_value), MB_text);
}
	
