/******************************************************************************
** ECE486/586 PDP-8 Simulator
** Sean Koppenhafer, Luis Santiago, Ken Benderly, J.S. Peirce
**
** cpu.c
**
******************************************************************************/

#include "cpu.h"
#include "memory.h"

/******************************************************************************
** OPCODE 0 - AND
******************************************************************************/
void AND(regs* registers) {
	registers->MB = mem_read(registers->CPMA);
	registers->AC &= registers->MB;
}

/******************************************************************************
** OPCODE 1 - TAD
******************************************************************************/
void TAD(regs* registers) {
	const uint16_t carry_out = 0x1000;	/* Check bit 13 for carry out */
	registers->MB = mem_read(registers->CPMA);
	registers->AC += registers->MB;

	if(registers->AC & carry_out) registers->link_bit = ~registers->link_bit;

	registers->AC &= CUTOFF_MASK;
}

/******************************************************************************
** OPCODE 2 - ISZ
******************************************************************************/
void ISZ(regs* registers) {
	registers->MB = mem_read(registers->CPMA);
	registers->MB = (registers->MB + 1) & CUTOFF_MASK;
	mem_write(registers->CPMA, registers->MB);

	if(!registers->MB) {
		registers->PC++;
	}
}

/******************************************************************************
** OPCODE 3 - DCA
******************************************************************************/
void DCA(regs* registers) {
	registers->MB = registers->AC;
	mem_write(registers->CPMA, registers->MB);
	registers->AC = 0;
}

/******************************************************************************
** OPCODE 4 - JMS
******************************************************************************/
void JMS(regs* registers) {
	registers->MB = registers->PC;	//PC will be incremented on return from subroutine
	mem_write(registers->CPMA, registers->MB);
	registers->PC = registers->CPMA & CUTOFF_MASK;	//PC will be incremented in main
																	//to give PC + 1
}

/******************************************************************************
** OPCODE 5 - JMP
******************************************************************************/
void JMP(regs* registers) {
	registers->PC = registers->CPMA;
}

/******************************************************************************
** OPCODE 7 GROUP 1 - CLA
******************************************************************************/
void CLA(regs* registers) {
	registers->AC = 0;
}

/******************************************************************************
** OPCODE 7 GROUP 1 - CLL
******************************************************************************/
void CLL(regs* registers) {
	registers->link_bit = 0;
}

/******************************************************************************
** OPCODE 7 GROUP 1 - CMA
******************************************************************************/
void CMA(regs* registers) {
	registers->AC = ~registers->AC & CUTOFF_MASK;
}

/******************************************************************************
** OPCODE 7 GROUP 1 - CML
******************************************************************************/
void CML(regs* registers) {
	registers->link_bit = ~registers->link_bit & 1;
}

/******************************************************************************
** OPCODE 7 GROUP 1 - IAC
******************************************************************************/
void IAC(regs* registers) {
	registers->AC = (registers->AC + 1) & CUTOFF_MASK;
}

/******************************************************************************
** OPCODE 7 GROUP 1 - RAR
******************************************************************************/
void RAR(regs* registers) {
	const uint8_t bit11_shift = 11;
	uint8_t old_link = registers->link_bit;

	/* Shift right 1.  New link is bit 0 of registers->AC.
	** Bit 11 of new registers->AC is old link bit value.
	*/
	registers->AC >>= 1;
	registers->link_bit = registers->AC & 1;
	registers->AC = (registers->AC | (old_link << bit11_shift)) & CUTOFF_MASK;
}

/******************************************************************************
** OPCODE 7 GROUP 1 - RTR
******************************************************************************/
void RTR(regs* registers) {
	/* Same as two RAR in a row */
	RAR(registers);
	RAR(registers);
}

/******************************************************************************
** OPCODE 7 GROUP 1 - RAL
******************************************************************************/
void RAL(regs* registers) {
	const uint8_t shift_num = 12;	/* Shift bit 12 into bit 0 */
	const uint16_t new_link_pos = 0x1000;	/* Bit 12 */

	/* Shift registers->AC left 1.  New link is bit 12
	** Bit 0 of registers->AC is old link bit
	*/
	registers->AC <<= 1;
	registers->AC |= registers->link_bit;
	registers->link_bit = (registers->AC & new_link_pos) >> shift_num;
	registers->AC &= CUTOFF_MASK;
}

/******************************************************************************
** OPCODE 7 GROUP 1 - RTL
******************************************************************************/
void RTL(regs* registers) {
	/* Same as two RAL in a row */
	RAL(registers);
	RAL(registers);
}

/******************************************************************************
** OPCODE 7 GROUP 2 - SMA
******************************************************************************/
void SMA(regs* registers) {
	const uint16_t sign_bit = 0x800;

	if(registers->AC & sign_bit) {
		registers->PC++;
	}
}

/******************************************************************************
** OPCODE 7 GROUP 2 - SZA
******************************************************************************/
void SZA(regs* registers) {
	if(!registers->AC) {
		registers->PC++;
	}
}

/******************************************************************************
** OPCODE 7 GROUP 2 - SNL
******************************************************************************/
void SNL(regs* registers) {
	if(registers->link_bit) {
		registers->PC++;
	}
}

/******************************************************************************
** OPCODE 7 GROUP 2 - SPA
******************************************************************************/
void SPA(regs* registers) {
	const uint16_t sign_bit = 0x800;

	if( !(registers->AC & sign_bit) ) {
		registers->PC++;
	}
}

/******************************************************************************
** OPCODE 7 GROUP 2 - SNA
******************************************************************************/
void SNA(regs* registers) {
	if(registers->AC) {
		registers->PC++;
	}
}

/******************************************************************************
** OPCODE 7 GROUP 2 - SZL
******************************************************************************/
void SZL(regs* registers) {
	if(!registers->link_bit) {
		registers->PC++;
	}
}

/******************************************************************************
** OPCODE 7 GROUP 2 - SKP
******************************************************************************/
void SKP(regs* registers) {
	registers->PC++;
}

/******************************************************************************
** OPCODE 7 GROUP 2 - OSR
******************************************************************************/
void OSR(regs* registers) {
	registers->AC = (registers->AC | registers->SR) & CUTOFF_MASK;
}

/******************************************************************************
** OPCODE 7 GROUP 2 - HLT
******************************************************************************/
void HLT(void) {
	printf("HALTING SYSTEM\n");
}

