/******************************************************************************
** ECE486/586 PDP-8 Simulator
** Sean Koppenhafer, Luis Santiago, Ken
**
** main.h
*/

#ifndef MAIN_H
#define MAIN_H

//#define FILL_DEBUG
#define DEBUG
#define DEBUG_GUI
#define GUI

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

typedef struct run_args {
	struct keyboard local_kb;
	g_items* object;
} run_args;

int main(int argc, char* argv[]);
void* run_program(void*);
void init_system(int argc, char* argv[]);
void fill_memory(int argc, char* argv[]);	/* Fills memory at bootup */
void print_stats(void);
void run_main( int argc, char*argv[], g_items * obj);

#endif
