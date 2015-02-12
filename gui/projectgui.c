#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
/****************************************************************************
                                   To DO 
*****************************************************************************
 1) maube different buttons 
 2) radio button 
 3) redirect g_item.buffer to receive a file
 4) get an input txt window for breakpoins
 5) add IR,
 6) add label for file name 
 7) maybe add a menu (only if we can make good use of it)
 8) add call back for buttons
 
***************************************************************************/
 typedef struct{
 // GtkWidget *window;
  GtkWidget *grid;
  GtkWidget *button1, * button2, *button3, *button4;
  GtkWidget *label, *PC_label, *Accumulator_label, *CPMA_label;
  GtkWidget *value, *PC_value, *Accumulator_value, *CPMA_value;
  GtkWidget *scrolled_window;
  GtkTextBuffer *buffer;
  GtkWidget *text_view;
  int c;
}g_items;


/*Signal handler for the "clicked" signal of the Button.
each click increment a variable by one an change label*/
static void on_button_click (GtkButton *button1,
                 gpointer   data)
{
g_items * tptr;
char buff[4];
tptr = (g_items *) data;
++(tptr->c);
// store int into buffer
sprintf(buff, "%d", tptr->c);
gtk_label_set_text(GTK_LABEL (tptr->PC_value),buff);
}



  /*Create a label and set its alignment. Setting the line wrap to TRUE makes 
  the label break lines if the text exceeds the widget's size. When set to 
  FALSE the text gets cut off by the edge of the widget*/
static void create_labels(g_items * obj)
{
  obj->label = gtk_label_new ("Registers");
  obj->PC_label= gtk_label_new ("PC");
  obj->Accumulator_label= gtk_label_new ("AC");
  obj->CPMA_label= gtk_label_new ("CPMA");

  gtk_label_set_justify (GTK_LABEL (obj->label), GTK_JUSTIFY_LEFT);
  gtk_label_set_justify (GTK_LABEL (obj->PC_label), GTK_JUSTIFY_LEFT);
  gtk_label_set_justify (GTK_LABEL (obj->Accumulator_label), GTK_JUSTIFY_LEFT);
  gtk_label_set_justify (GTK_LABEL (obj->CPMA_label), GTK_JUSTIFY_LEFT);

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

static void create_buttons(g_items* obj)
{
 
 obj->button1 = gtk_button_new_with_label ("Start");

 obj->button2 = gtk_button_new_with_label ("RUN");

 obj->button3 = gtk_button_new_with_label ("Continue");

 obj->button4 = gtk_button_new_with_label ("Step");
}

static void set_grid(g_items * obj)
{

int lcol ,c1row, c2row;
lcol =4;
c1row = c2row = 3;
/********************************************************************
  Create a grid and attach the button and progress bar
  accordingly
********************************************************************/
  obj->grid = gtk_grid_new ();// (left ,Top,with,height)
  gtk_grid_set_column_spacing (GTK_GRID (obj->grid), 10);
  gtk_grid_set_row_spacing (GTK_GRID (obj->grid), 3);
  gtk_grid_set_column_homogeneous (GTK_GRID (obj->grid), TRUE);
 // gtk_grid_set_row_homogeneous (GTK_GRID (grid), TRUE);

  gtk_grid_insert_column (GTK_GRID(obj->grid), 1);
  gtk_grid_insert_column (GTK_GRID(obj->grid), 2);
  gtk_grid_attach (GTK_GRID (obj->grid), obj->scrolled_window ,2, 3, 2, 15);
  gtk_widget_set_vexpand (obj->scrolled_window, TRUE);
  gtk_grid_attach (GTK_GRID (obj->grid), obj->button1 ,1, 1, 1, 1);

  gtk_grid_attach (GTK_GRID (obj->grid), obj->button1 ,1, 1, 1, 1);
  gtk_grid_attach_next_to (GTK_GRID (obj->grid),obj->button2, obj->button1,GTK_POS_RIGHT,1,1);

  gtk_grid_attach_next_to (GTK_GRID (obj->grid),obj->button3, obj->button2,GTK_POS_RIGHT,1,1);

  gtk_grid_attach_next_to (GTK_GRID (obj->grid),obj->button4,obj->button3,GTK_POS_RIGHT,1,1);

 
/****************************************************************************
NOTE: variables lcol and c1row are uto incremented to ensure that 
adding new things to the grid will be the one next to ot the on e
below it
*****************************************************************************/
  gtk_grid_attach (GTK_GRID (obj->grid), obj->label , lcol , c1row , 1, 1);
  gtk_grid_attach (GTK_GRID (obj->grid), obj->PC_label ,(lcol), ++c1row , 1, 1);
  gtk_grid_attach (GTK_GRID (obj->grid), obj->Accumulator_label ,lcol,
                    ++c1row , 1, 1);
  gtk_grid_attach (GTK_GRID (obj->grid), obj->CPMA_label ,lcol, ++c1row , 1, 1); 


  gtk_grid_attach (GTK_GRID (obj->grid), obj->value ,(++lcol), c2row , 1, 1);
  gtk_grid_attach (GTK_GRID (obj->grid), obj->PC_value, (lcol), ++c2row , 1, 1);
  gtk_grid_attach (GTK_GRID (obj->grid), obj->Accumulator_value ,lcol, ++c2row , 1, 1);
  gtk_grid_attach (GTK_GRID (obj->grid), obj->CPMA_value ,lcol, ++c2row, 1, 1);
}

static void activate (GtkApplication *app,
          gpointer    data)
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

/* text view window */

/*****************************************************************
    Text view is a widget in which can display the text buffer. 
    The line wrapping is set to break lines in between words.
*******************************************************************/
  /* The text buffer represents the text being edited */
  obj->buffer = gtk_text_buffer_new (NULL);   //input buffer
  obj->text_view = gtk_text_view_new_with_buffer (obj->buffer);
  gtk_text_view_set_wrap_mode (GTK_TEXT_VIEW (obj->text_view), GTK_WRAP_WORD);


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
  gtk_container_add (GTK_CONTAINER (obj->scrolled_window), 
                                         obj->text_view);
  gtk_container_set_border_width (GTK_CONTAINER (obj->scrolled_window), 5);

// call funtion that makes labels  
  create_labels(obj);
  create_buttons(obj);
  set_grid(obj);


 /*Connecting the clicked signal to the callback function*/
  
  g_signal_connect (GTK_BUTTON (obj->button1), "clicked", 
                    G_CALLBACK (on_button_click), obj);

  gtk_container_add (GTK_CONTAINER (window), GTK_WIDGET (obj->grid));
  gtk_widget_show_all (window);
}

int main (int argc, char **argv)
{
  GtkApplication *app;
  int status;
  g_items *obj = malloc(sizeof(*obj));
    
  app = gtk_application_new ("ECE.Project", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (activate), obj);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);


  free(obj);
  return status;
}
