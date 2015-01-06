/******************************************************************************
** ECE486/586 PDP-8 Simulator
** Sean Koppenhafer, Luis Santiago, Ken
**
** cpu.h
*/

#ifndef CPU_H
#define CPU_H

/* CPU registers - only 12 bits are used of the 16 */
uint16_t AC;		/* Accumulator */
uint16_t MQ;		/* Multiplier Quotient */
uint16_t PC;		/* Program Counter */
uint16_t MB;		/* Memory Buffer */
uint16_t CPMA;		/* Central Processor Memory Address */
uint16_t SR;		/* Console Switch Register */
uint8_t IR;			/* Instruction Register - only 3 bits are used */
uint8_t link_bit;	/* Carry out bit */

#endif

