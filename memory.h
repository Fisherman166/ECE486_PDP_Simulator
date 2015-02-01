/******************************************************************************
** ECE486/586 PDP-8 Simulator
** Sean Koppenhafer, Luis Santiago, Ken, J.S. Peirce
** 
** 15 JANUARY 2015
** memory.h
@CHANGELOG:
	JSP:
	-added tracefile open/close functions
	-TODO: talk to Luis about args passed for mem read/writes
******************************************************************************/
#define DEBUG 1
#include<inttypes.h>
#include<unistd.h>
#include <stdio.h>

#ifndef MEMORY_H
#define MEMORY_H

/* Total memory size is 4K */
#define WORDS_PER_PAGE 128
#define PAGES 32

#define MEMORY_MASK				0xFFF	/*Only 12 bits needed for address or a data*/
#define MEMORY_VALID_BIT 		0x8000
#define MEMORY_BREAKPOINT_BIT	0x4000


// Prototypes
void mem_init(void);
void mem_print_valid(void);
int trace_init();
int trace_close();
uint16_t mem_read(uint16_t to_convert);
void mem_write(uint16_t to_convert, uint16_t data);

uint16_t memory[PAGES * WORDS_PER_PAGE];
char *trace_name;
FILE *trace_file;

/******************************************************************************
**	EOF
******************************************************************************/
#endif

