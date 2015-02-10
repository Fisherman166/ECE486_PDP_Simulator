#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
/****************************************************************************
                                   To DO 
*****************************************************************************
 1) use struct to break widget into manageble functions
 2) redirect g_item.buffer to receive a file
 3) free obj (not freed as 2/10/2015)
 4) add a start and a quit button 
    - quit button MUST free obj
***************************************************************************/
 typedef struct{
 // GtkWidget *window;
  GtkWidget *grid;
  GtkWidget *button1;
  GtkWidget *label, *PC_label, *Accumulator_label, *CPMA_label;
  GtkWidget *value, *PC_value, *Accumulator_value, *CPMA_value;
  GtkWidget *scrolled_window;
  GtkTextBuffer *buffer;
  GtkWidget *text_view;
  int c;
}g_items;


/*Signal handler for the "clicked" signal of the Button. ntiEach
click generates a progress bar pulse*/
static void on_button_click (GtkButton *button1,
                 gpointer   data)
{
int n;
g_items * tptr;
char buff[4];

tptr = (g_items *) data;
++(tptr->c);
// store int into buffer
sprintf(buff, "%d", tptr->c);
gtk_label_set_text(GTK_LABEL (tptr->PC_value),buff);
}


static void activate (GtkApplication *app,
          gpointer    data)
{
  GtkWidget *window;
g_items *obj = malloc(sizeof(*obj));

obj->c =0;
/******************************************************************
                    create window and set size
******************************************************************/
  window = gtk_application_window_new (app);
  gtk_window_set_default_size (GTK_WINDOW (window), 900, 800);
  gtk_window_set_title (GTK_WINDOW (window), "ECE486/586 PDP8 Project");
  obj->button1 = gtk_button_new_with_label ("Start");

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



  /*Create a label and set its alignment. Setting the line wrap to TRUE makes 
  the label break lines if the text exceeds the widget's size. When set to 
  FALSE the text gets cut off by the edge of the widget*/
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
  gtk_label_set_justify (GTK_LABEL (obj->PC_value), GTK_JUSTIFY_LEFT);
  gtk_label_set_justify (GTK_LABEL (obj->Accumulator_value), GTK_JUSTIFY_LEFT);
  gtk_label_set_justify (GTK_LABEL (obj->CPMA_value), GTK_JUSTIFY_LEFT);

 
/*   add line wraps if wanted */
// gtk_label_set_line_wrap (GTK_LABEL (obj->label), TRUE);



/********************************************************************
  Create a grid and attach the button and progress bar
  accordingly
********************************************************************/
  obj->grid = gtk_grid_new ();// (left ,Top,with,height)
  gtk_grid_set_column_spacing (GTK_GRID (obj->grid), 10);
  gtk_grid_set_row_spacing (GTK_GRID (obj->grid), 2);
  gtk_grid_set_column_homogeneous (GTK_GRID (obj->grid), TRUE);
 // gtk_grid_set_row_homogeneous (GTK_GRID (grid), TRUE);
  
  gtk_grid_attach (GTK_GRID (obj->grid),obj->scrolled_window, 1, 1 , 1, 12);

  gtk_grid_attach (GTK_GRID (obj->grid), obj->label ,2, 1 , 1, 1);
  gtk_grid_attach (GTK_GRID (obj->grid), obj->PC_label ,2, 2 , 1, 1);
  gtk_grid_attach (GTK_GRID (obj->grid), obj->Accumulator_label ,2, 3 , 1, 1);
  gtk_grid_attach (GTK_GRID (obj->grid), obj->CPMA_label ,2, 4 , 1, 1); 
  gtk_grid_attach (GTK_GRID (obj->grid), obj->button1 ,2, 5 , 1, 1);


  gtk_grid_attach (GTK_GRID (obj->grid), obj->value ,3, 1 , 1, 1);
  gtk_grid_attach (GTK_GRID (obj->grid), obj->PC_value ,3, 2 , 1, 1);
  gtk_grid_attach (GTK_GRID (obj->grid), obj->Accumulator_value ,3, 3 , 1, 1);
  gtk_grid_attach (GTK_GRID (obj->grid), obj->CPMA_value ,3, 4 , 1, 1);

 /*Connecting the clicked signal to the callback function*/
  
  g_signal_connect (GTK_BUTTON (obj->button1), "clicked", 
                    G_CALLBACK (on_button_click), obj);
  g_print(" c value in activate: %d", obj->c);
  g_printf("2value of obj addr -> %p \n",obj);

  gtk_container_add (GTK_CONTAINER (window), GTK_WIDGET (obj->grid));
  gtk_widget_show_all (window);
}

int main (int argc, char **argv)
{
  GtkApplication *app;
  int status;

  app = gtk_application_new ("ECE.Project", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);
  return status;
}
