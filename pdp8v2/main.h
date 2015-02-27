/******************************************************************************
** ECE486/586 PDP-8 Simulator
** Sean Koppenhafer, Luis Santiago, Ken
**
** main.h
*/

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

int main(int argc, char* argv[]);
void* run_program(void*);
void execute_opcode(g_items* object);
void init_system(int argc, char* argv[], g_items*);
void fill_memory(int argc, char* argv[]);	/* Fills memory at bootup */
void print_stats(void);
void shutdown_system(g_items*);

#endif
