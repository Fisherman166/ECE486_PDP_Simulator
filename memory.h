/******************************************************************************
** ECE486/586 PDP-8 Simulator
** Sean Koppenhafer, Luis Santiago, Ken
**
** memory.h
*/

#include <inttypes.h>

#ifndef MEMORY_H
#define MEMORY_H

/* Total memory size is 4K */
#define WORDS_PER_PAGE 128
#define PAGES 32
#define MEMORY_MASK 0xFFF	/* Only 12 bits are needed in an address or a data item */

uint16_t memory[PAGES][WORDS_PER_PAGE];

#endif

