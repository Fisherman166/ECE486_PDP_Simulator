/******************************************************************************
** ECE486/586 PDP-8 Simulator
** Sean Koppenhafer, Luis Santiago, Ken Benderly, J.S. Peirce
**
** cpu.h
**
******************************************************************************/

#ifndef CPU_H
#define CPU_H

#include <stdio.h>
#include <inttypes.h>
#include <assert.h>

#define CUTOFF_MASK 0xFFF	/* Bitmask to keep registers/memory at 12 bits */

/* CPU registers - only 12 bits are used of the 16 */
typedef struct {
	uint16_t AC;		/* Accumulator */
	uint16_t MQ;		/* Multiplier Quotient */
	uint16_t PC;		/* Program Counter */
	uint16_t MB;		/* Memory Buffer */
	uint16_t CPMA;		/* Central Processor Memory Address */
	uint16_t SR;		/* Console Switch Register */
	uint8_t IR;			/* Instruction Register - only 3 bits are used */
	uint8_t link_bit;	/* Carry out bit */
} regs;

/* Opcodes 0-5 - Memory reference functions */
void AND(regs*);
void TAD(regs*);
void ISZ(regs*);
void DCA(regs*);
void JMS(regs*);
void JMP(regs*);

/* Opcode 7 - group 1 */
void CLA(regs*);
void CLL(regs*);
void CMA(regs*);
void CML(regs*);
void IAC(regs*);
void RAR(regs*);
void RTR(regs*);
void RAL(regs*);
void RTL(regs*);

/* Opcoderegs* 7 - group 2 */
void SMA(regs*);
void SZA(regs*);
void SNL(regs*);
void SPA(regs*);
void SNA(regs*);
void SZL(regs*);
void SKP(regs*);
void OSR(regs*);
void HLT(void);

/* Test functions */
void reset_test_regs(regs*);
void group1_opcodes_test(regs*);
void group1_micro_test(regs*);
void group2_micro_test(regs*);

#endif

