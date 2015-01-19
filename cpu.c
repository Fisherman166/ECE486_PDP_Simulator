/******************************************************************************
** ECE486/586 PDP-8 Simulator
** Sean Koppenhafer, Luis Santiago, Ken Benderly, J.S. Peirce
**
** cpu.c
**
******************************************************************************/

#include "cpu.h"

/******************************************************************************
** OPCODE 0 - AND
******************************************************************************/
void AND(regs* registers) {
	registers->AC &= registers->MB;
}

/******************************************************************************
** OPCODE 1 - TAD
******************************************************************************/
void TAD(regs* registers) {
	const uint16_t carry_out = 0x1000;	/* Check bit 13 for carry out */
	registers->AC += registers->MB;

	if(registers->AC & carry_out) registers->link_bit = ~registers->link_bit;

	registers->AC &= CUTOFF_MASK;
}

/******************************************************************************
** OPCODE 2 - ISZ
******************************************************************************/
void ISZ(regs* registers) {
	registers->MB = (registers->MB + 1) & CUTOFF_MASK;

	if(!registers->MB) {
		registers->PC++;
	}
}

/******************************************************************************
** OPCODE 3 - DCA
******************************************************************************/
void DCA(regs* registers) {
	registers->MB = registers->AC;
	registers->AC = 0;
}

/******************************************************************************
** OPCODE 4 - JMS
******************************************************************************/
void JMS(regs* registers) {
	registers->MB = registers->PC;
	/* TODO: write MB to memory */
	registers->PC = (registers->CPMA + 1) & CUTOFF_MASK;
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


/******************************************************************************
** TEST MAIN FOR TESTING THE OPCODES
******************************************************************************/
int main() {
	regs registers;

	reset_test_regs(&registers);
	group1_opcodes_test(&registers);

	reset_test_regs(&registers);
	group1_micro_test(&registers);

	reset_test_regs(&registers);
	group2_micro_test(&registers);

	return 0;
}

void reset_test_regs(regs* registers) {
	registers->AC = 0;
	registers->PC = 0;
	registers->MB = 0;
	registers->CPMA = 0;
}

void group1_opcodes_test(regs* registers) {
	/* AND test */
	registers->AC = 0xFFF;
	registers->MB = 0xF1C;
	AND(registers);
	assert(registers->AC == 0xF1C);

	/* TAD test: -25 + -26 */
	registers->AC = 0xFE7; 	//-25
	registers->MB = 0xFE6;	//-26
	TAD(registers);
	assert(registers->AC == 0xFCD); /* -51 */

	/* TAD test: 25 + -26 */
	registers->AC = 0xFE6;	//-26 
	registers->MB = 0x19;		//25
	TAD(registers);
	assert(registers->AC == 0xFFF); /* -1 */

	/* TAD test: 26 + -25 */
	registers->AC = 0x1A;	//26 
	registers->MB = 0xFE7;	//-25
	TAD(registers);
	assert(registers->AC == 0x1); /* 1 */

	/* TAD test: 25 + 26 */
	registers->AC = 0x19;		//25
	registers->MB = 0x1A;		//26
	TAD(registers);
	assert(registers->AC == 0x33); /* 51 */

	/* ISZ test */
	registers->MB = 0xFFF;
	ISZ(registers);
	assert(registers->MB == 0);
	assert(registers->PC == 1);

	/* DCA test */
	registers->AC = 0xFFE;
	DCA(registers);
	assert(registers->AC == 0);
	assert(registers->MB == 0xFFE);

	/* JMS test */
	registers->CPMA = 0x10;
	JMS(registers);
	assert(registers->PC == 0x11);
	assert(registers->MB == 0x1);

	/* JMP test */
	registers->CPMA = 0x25;
	JMP(registers);
	assert(registers->PC == 0x25);
}

void group1_micro_test(regs* registers) {
	/* CLA test */
	registers->AC = 0x83D;
	CLA(registers);
	assert(registers->AC == 0);

	/* CLL test */
	registers->link_bit = 0x1;
	CLL(registers);
	assert(registers->link_bit == 0);

	/* CMA test */
	registers->AC = 0;
	CMA(registers);
	assert(registers->AC == 0xFFF);

	/* CML test */
	registers->link_bit = 0;
	CML(registers);
	assert(registers->link_bit == 1);

	/* IAC test */
	registers->AC = 0;
	IAC(registers);
	assert(registers->AC == 0x1);

	/* RAR test */
	registers->link_bit = 1;
	registers->AC = 0xFFD;
	RAR(registers);
	assert(registers->link_bit == 0);
	assert(registers->AC == 0xFFE);

	/* RTR test */
	registers->link_bit = 0;
	registers->AC = 0xFFD;
	RTR(registers);
	assert(registers->link_bit == 1);
	assert(registers->AC == 0x3FF);

	/* RAL test */
	registers->link_bit = 1;
	registers->AC = 0x7FE;
	RAL(registers);
	assert(registers->link_bit == 0);
	assert(registers->AC == 0xFFD);

	/* RTR test */
	registers->link_bit = 1;
	registers->AC = 0x7FE;
	RTL(registers);
	assert(registers->link_bit == 1);
	assert(registers->AC == 0xFFA);
}

void group2_micro_test(regs* registers) {
	/* FOR ALL OF THESE TESTS, FIRST ONE FAILS AND SECOND WORKS */

	/* SMA test */
	registers->AC = 15;
	SMA(registers);
	assert(registers->PC == 0);
	registers->AC = 0xFFB;	/* -5 */
	SMA(registers);
	assert(registers->PC == 1);

	/* SZA test */
	registers->AC = 15;
	SZA(registers);
	assert(registers->PC == 1);
	registers->AC = 0;
	SZA(registers);
	assert(registers->PC == 2);

	/* SNL test */
	registers->link_bit = 0;
	SNL(registers);
	assert(registers->PC == 2);
	registers->link_bit = 1;
	SNL(registers);
	assert(registers->PC == 3);

	/* SPA test */
	registers->AC = 0xFC2;
	SPA(registers);
	assert(registers->PC == 3);
	registers->AC = 15;
	SPA(registers);
	assert(registers->PC == 4);

	/* SNA test */
	registers->AC = 0x0;
	SNA(registers);
	assert(registers->PC == 4);
	registers->AC = 15;
	SNA(registers);
	assert(registers->PC == 5);
	registers->AC = 0xFFB;
	SNA(registers);
	assert(registers->PC == 6);

	/* SZL test */
	registers->link_bit = 1;
	SZL(registers);
	assert(registers->PC == 6);
	registers->link_bit = 0;
	SZL(registers);
	assert(registers->PC == 7);

	/* SKP test */
	SKP(registers);
	assert(registers->PC == 8);

	/* OSR test */
	registers->AC = 0;
	registers->SR = 0xFC7;
	OSR(registers);
	assert(registers->AC == 0xFC7);

	HLT();
}

