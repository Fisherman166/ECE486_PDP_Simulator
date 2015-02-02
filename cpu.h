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

#define OP_CODE_AND		0
#define OP_CODE_TAD		01000
#define OP_CODE_ISZ		02000
#define OP_CODE_DCA		03000
#define OP_CODE_JMS		04000
#define OP_CODE_JMP		05000
#define OP_CODE_IO		06000
#define OP_CODE_MICRO	07000

#define MICRO_INSTRUCTION_GROUP_BIT 0400
#define MICRO_INSTRUCTION_BITS_MASK 0377

// Group 1 micro instructions
#define MICRO_INSTRUCTION_CLA_BITS	0200
#define MICRO_INSTRUCTION_CLL_BITS	0100
#define MICRO_INSTRUCTION_CMA_BITS	0040
#define MICRO_INSTRUCTION_CML_BITS	0020
#define MICRO_INSTRUCTION_IAC_BITS	0001
#define MICRO_INSTRUCTION_RAR_BITS	0010
#define MICRO_INSTRUCTION_RTR_BITS	0012
#define MICRO_INSTRUCTION_RAL_BITS	0004
#define MICRO_INSTRUCTION_RTL_BITS	0006

// Group 2 micro instructions
#define MICRO_INSTRUCTION_SMA_BITS	0100
#define MICRO_INSTRUCTION_SZA_BITS	0040
#define MICRO_INSTRUCTION_SNL_BITS	0020
#define MICRO_INSTRUCTION_SPA_BITS	0110
#define MICRO_INSTRUCTION_SNA_BITS	0050
#define MICRO_INSTRUCTION_SZL_BITS	0030
#define MICRO_INSTRUCTION_SKP_BITS	0010
#define MICRO_INSTRUCTION_CLA_BITS	0200
#define MICRO_INSTRUCTION_OSR_BITS	0004
#define MICRO_INSTRUCTION_HLT_BITS	0002

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

