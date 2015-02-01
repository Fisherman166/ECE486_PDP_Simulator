/******************************************************************************
** ECE486/586 PDP-8 Simulator
** Sean Koppenhafer, Luis Santiago, Ken
**
** main.h
*/

#ifndef MAIN_H
#define MAIN_H

//#define FILL_DEBUG

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char* argv[]);
void fill_memory(int argc, char* argv[]);	/* Fills memory at bootup */
void run_program(void);

#endif
