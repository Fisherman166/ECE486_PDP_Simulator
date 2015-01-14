/******************************************************************************
** ECE486/586 PDP-8 Simulator
** Sean Koppenhafer, Luis Santiago, Ken Benderly, J.S. Peirce
**
** cpu.h
*/

#ifndef CPU_H
#define CPU_H

#define CUTOFF_MASK 0xFFF	/* Bitmask to keep registers/memory at 12 bits */

/* CPU registers - only 12 bits are used of the 16 */
uint16_t AC;		/* Accumulator */
uint16_t MQ;		/* Multiplier Quotient */
uint16_t PC;		/* Program Counter */
uint16_t MB;		/* Memory Buffer */
uint16_t CPMA;		/* Central Processor Memory Address */
uint16_t SR;		/* Console Switch Register */
uint8_t IR;			/* Instruction Register - only 3 bits are used */
uint8_t link_bit;	/* Carry out bit */

/* Opcodes 0-5 - Memory reference functions */
void AND(void);
void TAD(void);
void ISZ(void);
void DCA(void);
void JMS(void);
void JMP(void);

/* Opcode 7 - group 1 */
void CLA(void);
void CLL(void);
void CMA(void);
void CML(void);
void IAC(void);
void RAR(void);
void RTR(void);
void RAL(void);
void RTL(void);

#endif

