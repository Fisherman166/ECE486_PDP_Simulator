/******************************************************************************
** ECE486/586 PDP-8 Simulator
** Sean Koppenhafer, Luis Santiago, Ken Benderly, J.S. Peirce
** 
** main.h
******************************************************************************/

#ifndef MAIN_H
#define MAIN_H

//#define FILL_DEBUG
//#define DEBUG
#define DEBUG_GUI
//#define GUI

#ifdef GUI
#include <gtk/gtk.h>
#include "gui.h"
#include "callback.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "memory.h"

#include "kb_input.h"
#include "branch_trace.h"

#define OPCODE_NUM 8
#define STEP 1
#define RUN 0

//This struct holds variables that both the GUI and non GUI versio need
typedef struct shared_vars {
	regs *registers_ptr;
   struct keyboard* kb_ptr;
	uint8_t breakpoint_reached;
	uint8_t breakpoint_serviced;	//Used to run again after a breakpoint was reached
	uint8_t step_or_run;
	uint8_t execution_done;
	uint8_t ran_once;
	uint8_t branch_trace;	//Will be a 1 if branch trace last screen loaded
	int last_mem_page;
} shared_vars;

int main(int argc, char* argv[]);
void run_no_GUI(int, char**);
void* run_program(void*);
void execute_opcode(struct shared_vars*);
void init_system(int argc, char* argv[], struct shared_vars*);
void fill_memory(int argc, char* argv[]);	/* Fills memory at bootup */
void print_stats(void);
void shutdown_system(struct shared_vars*);
void remove_text_files(char * buffer);

#endif
