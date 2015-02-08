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

#define OPCODE_NUM 8

static uint32_t clock_cycles = 0;
static uint32_t opcode_freq[OPCODE_NUM];
static uint8_t opcode_cycles[OPCODE_NUM] = {2,2,2,2,2,1,0,1};
static const char *opcode_text[OPCODE_NUM];

int main(int argc, char* argv[]);
void run_program(void);
void init_system(int argc, char* argv[]);
void fill_memory(int argc, char* argv[]);	/* Fills memory at bootup */
void print_stats(void);

#endif
