// gui.c

#include "gui.h"
#include "callback.h"

// compile with  
// gcc projgui.c -o proj $(pkg-config --cflags --libs gtk+-3.0)

/****************************************************************************
                                   To DO
*****************************************************************************
 1) maube different buttons
 2) add IR, CPMA, other registers to lables 
 3) maybe add a menu (only if we can make good use of it)
 4) msg window 
 5) 
 6)
 7) 
 8) 
 9) 
***************************************************************************/



/*Signal handler for the "clicked" signal of the Button.
each click increment a variable by one an change label*/




/*Create a label and set its alignment. Setting the line wrap to TRUE makes
the label break lines if the text exceeds the widget's size. When set to
FALSE the text gets cut off by the edge of the widget*/
void create_labels(g_items * obj)
{
    obj->pagenum= gtk_label_new ("Select a Page Number");
    obj->label = gtk_label_new ("Registers");
    obj->PC_label= gtk_label_new ("PC");
    obj->Accumulator_label= gtk_label_new ("AC");
    obj->CPMA_label= gtk_label_new ("CPMA");
    obj->Mesages_label= gtk_label_new ("Messages");

    gtk_label_set_justify (GTK_LABEL (obj->pagenum), GTK_JUSTIFY_CENTER);
    gtk_label_set_justify (GTK_LABEL (obj->label), GTK_JUSTIFY_LEFT);
    gtk_label_set_justify (GTK_LABEL (obj->PC_label), GTK_JUSTIFY_LEFT);
    gtk_label_set_justify (GTK_LABEL (obj->Accumulator_label), GTK_JUSTIFY_LEFT);
    gtk_label_set_justify (GTK_LABEL (obj->CPMA_label), GTK_JUSTIFY_LEFT);
    gtk_label_set_justify (GTK_LABEL (obj->Mesages_label), GTK_JUSTIFY_CENTER);

// values

    obj->value = gtk_label_new ("values");
    obj->PC_value= gtk_label_new ("PC value");
    obj->Accumulator_value= gtk_label_new ("AC value");
    obj->CPMA_value = gtk_label_new ("CPMA value");

    gtk_label_set_justify (GTK_LABEL (obj->value), GTK_JUSTIFY_LEFT);
    gtk_label_set_justify (GTK_LABEL (obj->PC_value), GTK_JUSTIFY_RIGHT);
    gtk_label_set_justify (GTK_LABEL (obj->Accumulator_value), GTK_JUSTIFY_LEFT);
    gtk_label_set_justify (GTK_LABEL (obj->CPMA_value), GTK_JUSTIFY_RIGHT);
}

void create_buttons(g_items* obj)
{
// radio button needs to be created and label created


    obj->run_button = gtk_button_new_with_label ("RUN");

    obj->button3 = gtk_button_new_with_label ("Continue");

    obj->button4 = gtk_button_new_with_label ("Step");

 /* Create an adjustment representing an adjustable bounded value */
  obj->adjustment = gtk_adjustment_new (0, 0, 31, 1, 0, 0);

/* Create a spin button that is to be as wide as possible */
  obj->spin_button = gtk_spin_button_new (obj->adjustment, 1, 0);
 // gtk_widget_set_hexpand (spin_button, TRUE);


}

void create_entrybox (g_items* obj)
{
    obj->BreakP_entry = gtk_entry_new();
    obj->Trace_entry = gtk_entry_new();
}




void set_grid(g_items * obj)
{

    int lcol ,c1row, c2row;
    lcol =3;
    c1row = c2row = 2;
    /********************************************************************
      Create a grid and attach the button and progress bar
      accordingly
    ********************************************************************/
    obj->grid = gtk_grid_new ();
    gtk_grid_set_column_spacing (GTK_GRID (obj->grid), 10);
    gtk_grid_set_row_spacing (GTK_GRID (obj->grid), 3);
    gtk_grid_set_column_homogeneous (GTK_GRID (obj->grid), TRUE);
// gtk_grid_set_row_homogeneous (GTK_GRID (grid), TRUE);




//***********************************************************(left ,Top, width, height)
    gtk_widget_set_vexpand (obj->scrolled_window, TRUE);
 //   gtk_grid_attach (GTK_GRID (obj->grid), obj->pagenum, 1, 1, 1, 1);
    gtk_grid_attach (GTK_GRID (obj->grid), obj->scrolled_window ,1, 4, 2, 10);


    /* set button location on the grid */
    gtk_grid_attach (GTK_GRID (obj->grid), obj->run_button ,1, 1, 1, 1);
    gtk_grid_attach_next_to (GTK_GRID (obj->grid),obj->button3, obj->run_button,GTK_POS_RIGHT,1,1);
    gtk_grid_attach_next_to (GTK_GRID (obj->grid),obj->button4, obj->button3,GTK_POS_RIGHT,1,1);
    gtk_grid_attach_next_to (GTK_GRID (obj->grid),obj->pagenum, obj->run_button,GTK_POS_BOTTOM,1,1);
    gtk_grid_attach_next_to (GTK_GRID (obj->grid),obj->spin_button, obj->pagenum,GTK_POS_BOTTOM,1,1);




    /****************************************************************************
    NOTE: variables lcol and c1row are uto incremented to ensure that
    adding new things to the grid will be the one next to ot the on e
    below it
    *****************************************************************************/

//***********************************************************(left ,Top, width, height)
    gtk_grid_attach (GTK_GRID (obj->grid), obj->label , lcol , c1row , 1, 1);
    gtk_grid_attach (GTK_GRID (obj->grid), obj->PC_label , lcol, ++c1row , 1, 1);
    gtk_grid_attach (GTK_GRID (obj->grid), obj->Accumulator_label ,lcol,
                     ++c1row , 1, 1);
    gtk_grid_attach (GTK_GRID (obj->grid), obj->CPMA_label ,lcol, ++c1row , 1, 1);

    gtk_grid_attach (GTK_GRID (obj->grid), obj->value    ,(++lcol), c2row , 1, 1);
    gtk_grid_attach (GTK_GRID (obj->grid), obj->PC_value, (lcol), ++c2row , 1, 1);
    gtk_grid_attach (GTK_GRID (obj->grid), obj->Accumulator_value ,lcol, ++c2row , 1, 1);
    gtk_grid_attach (GTK_GRID (obj->grid), obj->CPMA_value ,lcol, ++c2row, 1, 1);



// radio Buttons placement
    gtk_grid_attach (GTK_GRID (obj->grid), obj->radio_set_BP ,lcol, ++c2row , 1, 1);
    gtk_grid_attach_next_to (GTK_GRID (obj->grid),obj->radio_clear_BP, obj->radio_set_BP,GTK_POS_BOTTOM,1,1);
    gtk_grid_attach_next_to (GTK_GRID (obj->grid),obj->BreakP_entry,obj->radio_clear_BP, GTK_POS_BOTTOM, 1,1);

    gtk_grid_attach (GTK_GRID (obj->grid), obj->radioB_SetTr ,lcol-1, c2row , 1, 1);
    gtk_grid_attach_next_to (GTK_GRID (obj->grid),obj->radioB_ClrTr, obj->radioB_SetTr,GTK_POS_BOTTOM,1,1);

	  gtk_grid_attach_next_to (GTK_GRID (obj->grid),obj->Trace_entry  , obj->radioB_ClrTr,GTK_POS_BOTTOM,1,1);


	gtk_grid_attach (GTK_GRID (obj->grid), obj->Mesages_label, lcol-1 , c1row+4 , 2, 1);

// message window 
gtk_grid_attach (GTK_GRID (obj->grid), obj->scrolled_msg ,lcol-1, c1row+5, 2, 2);
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

}


 void create_text_boxes (g_items * obj)
{
// main box to the left 
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


    /* The function directly below is used to add children to the scrolled window
     * with scrolling capabilities (e.g text_view), otherwise,
     * gtk_scrolled_window_add_with_viewport() would have been used
     */
    gtk_container_add (GTK_CONTAINER (obj->scrolled_msg), obj->messages_txt);
    gtk_container_set_border_width (GTK_CONTAINER (obj->scrolled_msg), 5);




}

void create_radio_buttons(g_items* obj)
{
	obj->radio_set_BP=gtk_radio_button_new_with_label(NULL, "Set Breakpoints");
    
	obj->radio_clear_BP=gtk_radio_button_new_with_label_from_widget 
	(GTK_RADIO_BUTTON (obj->radio_set_BP), "Clear Breakpoints");

    	obj->radioB_SetTr =gtk_radio_button_new_with_label(NULL, "Set Trace");
    
    	obj->radioB_ClrTr =gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON (obj->radioB_SetTr), "Clear Trace");


}



void set_radio_buttons( g_items *obj)
{
// set values 
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (obj->radio_set_BP), TRUE);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (obj->radio_clear_BP), FALSE);

    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (obj->radioB_SetTr), TRUE);
    gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (obj->radioB_ClrTr), FALSE);
}
void entry_box_cb(g_items *obj)
{
 g_signal_connect (GTK_ENTRY (obj->BreakP_entry), "activate", 
                    G_CALLBACK (new_breakpoint), obj);

 g_signal_connect (GTK_ENTRY (obj->Trace_entry), "activate", 
                    G_CALLBACK (new_tracepoint), obj);

}

void radio_button_callbacks( g_items * obj)
{

  g_signal_connect (GTK_TOGGLE_BUTTON (obj->radio_set_BP), "toggled", 
                    G_CALLBACK (set_breakpt_cb),  obj);
  g_signal_connect (GTK_TOGGLE_BUTTON (obj->radio_clear_BP), "toggled", 
                    G_CALLBACK (clear_breakpt_cb),  obj);

// trace points
  g_signal_connect (GTK_TOGGLE_BUTTON (obj->radioB_SetTr), "toggled", 
                    G_CALLBACK (set_tracepoint),  obj);
  g_signal_connect (GTK_TOGGLE_BUTTON (obj->radioB_ClrTr), "toggled", 
                    G_CALLBACK (clear_tracepoint),  obj);


}



void create_buttons_callbacks(g_items* obj)
{
   /*Connecting the clicked signal to the callback function*/

    g_signal_connect (GTK_BUTTON (obj->run_button), "clicked",
                      G_CALLBACK (run_button_click), obj);

    g_signal_connect (obj->spin_button, 
                    "value-changed", 
                    G_CALLBACK (spin_clicked), 
                    obj);
}
 



 void activate (GtkApplication *app, gpointer    data)
{
    GtkWidget *window;

    g_items * obj;

    obj = (g_items *) data;

    obj->c =0;


    /******************************************************************
                        create window and set size
    ******************************************************************/
    window = gtk_application_window_new (app);
    gtk_window_set_default_size (GTK_WINDOW (window), 900, 800);
    gtk_window_set_title (GTK_WINDOW (window), "ECE486/586 PDP8 Project");


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
    
 

    gtk_container_add (GTK_CONTAINER (window), GTK_WIDGET (obj->grid));
    gtk_widget_show_all (window);
}
