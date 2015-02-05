/******************************************************************************
** ECE486/586 PDP-8 Simulator
** Sean Koppenhafer, Luis Santiago, Ken
**
** main.c
*/

#include "main.h"
#include "memory.h"
#include "cpu.h"

#define OP_CODE_MASK 07000		// bits 0,1,2
//#define OP_CODE_SHIFT 9			// how many bits to shift op code into lsb's

static uint32_t clock_cycles = 0;
static uint32_t opcode_freq[OPCODE_NUM];
static uint8_t opcode_cycles[OPCODE_NUM] = {2,2,2,2,2,1,0,1};

int main(int argc, char* argv[]) {
	int trace_return;
	init_system(argc, argv);

	//can capture return values to verify fopen && fclose
	trace_return = trace_init();
	if(trace_return) {
		printf("Unable to open trace file. Exiting\n");
		exit(-1);
	}

	run_program();
	mem_print_valid();
	print_stats();
	trace_close();
	return(0);
}/*end main*/

/******************************************************************************
** 	RUN THE PROGRAM IN MEMORY 	
******************************************************************************/
void run_program(void){
	uint16_t current_instruction;
	uint8_t addressing_mode;
	regs registers;
	
	reset_regs(&registers);		// initialize the CPU 

	do {
		current_instruction = mem_read(registers.PC, INSTRUCTION_FETCH);	// load the next instruction
		addressing_mode = EffAddCalc(current_instruction, &registers);		// Load the CPMA with effective address
		registers.IR = (current_instruction >> 9) & 0xFF;						// Put the opcode in here

		/* !update trace file here for instruction read! */
		switch(current_instruction & OP_CODE_MASK){
		case OP_CODE_AND:
			AND(&registers);
			opcode_freq[0]++;
			break;
		case OP_CODE_TAD:
			TAD(&registers);
			opcode_freq[1]++;
			break;
		case OP_CODE_ISZ:
			ISZ(&registers);
			opcode_freq[2]++;
			break;
		case OP_CODE_DCA:
			DCA(&registers);
			opcode_freq[3]++;
			break;
		case OP_CODE_JMS:
			JMS(&registers);
			opcode_freq[4]++;
			break;
		case OP_CODE_JMP:
			JMP(&registers);
			opcode_freq[5]++;
			break;
		case OP_CODE_IO:
			printf("Op code 6? Implement I/O first\n");
			opcode_freq[6]++;
			break;
		case OP_CODE_MICRO:
			opcode_freq[7]++;
			switch(current_instruction & MICRO_INSTRUCTION_GROUP_BIT){
			case 0:	// Group 1
				// Go through the bits and exeucute in sequence if high
				if((current_instruction & MICRO_INSTRUCTION_BITS_MASK) == 0){
					break;	// NOP instruction
				}
				if((current_instruction & MICRO_INSTRUCTION_CLA_BITS) == MICRO_INSTRUCTION_CLA_BITS){
					CLA(&registers);
				}
				if((current_instruction & MICRO_INSTRUCTION_CLL_BITS) == MICRO_INSTRUCTION_CLL_BITS){
					CLL(&registers);
				}
				if((current_instruction & MICRO_INSTRUCTION_CMA_BITS) == MICRO_INSTRUCTION_CMA_BITS){
					CMA(&registers);
				}
				if((current_instruction & MICRO_INSTRUCTION_CML_BITS) == MICRO_INSTRUCTION_CML_BITS){
					CML(&registers);
				}
				if((current_instruction & MICRO_INSTRUCTION_IAC_BITS) == MICRO_INSTRUCTION_IAC_BITS){
					IAC(&registers);
				}
				if((current_instruction & MICRO_INSTRUCTION_RAR_BITS) == MICRO_INSTRUCTION_RAR_BITS){
					RAR(&registers);
				}
				if((current_instruction & MICRO_INSTRUCTION_RTR_BITS) == MICRO_INSTRUCTION_RTR_BITS){
					RTR(&registers);
				}
				if((current_instruction & MICRO_INSTRUCTION_RAL_BITS) == MICRO_INSTRUCTION_RAL_BITS){
					RAL(&registers);
				}
				if((current_instruction & MICRO_INSTRUCTION_RTL_BITS) == MICRO_INSTRUCTION_RTL_BITS){
					RTL(&registers);
				}
				break;

			case MICRO_INSTRUCTION_GROUP_BIT:	// Group 2
				if((current_instruction & MICRO_INSTRUCTION_SMA_BITS) == MICRO_INSTRUCTION_SMA_BITS){
					SMA(&registers);
				}
				if((current_instruction & MICRO_INSTRUCTION_SZA_BITS) == MICRO_INSTRUCTION_SZA_BITS){
					SZA(&registers);
				}
				if((current_instruction & MICRO_INSTRUCTION_SNL_BITS) == MICRO_INSTRUCTION_SNL_BITS){
					SNL(&registers);
				}
				if((current_instruction & MICRO_INSTRUCTION_SPA_BITS) == MICRO_INSTRUCTION_SPA_BITS){
					SPA(&registers);
				}
				if((current_instruction & MICRO_INSTRUCTION_SNA_BITS) == MICRO_INSTRUCTION_SNA_BITS){
					SNA(&registers);
				}
				if((current_instruction & MICRO_INSTRUCTION_SZL_BITS) == MICRO_INSTRUCTION_SZL_BITS){
					SZL(&registers);
				}
				if((current_instruction & MICRO_INSTRUCTION_SKP_BITS) == MICRO_INSTRUCTION_SKP_BITS){
					SKP(&registers);
				}
				if((current_instruction & MICRO_INSTRUCTION_CLA_BITS) == MICRO_INSTRUCTION_CLA_BITS){
					CLA(&registers);
				}
				if((current_instruction & MICRO_INSTRUCTION_OSR_BITS) == MICRO_INSTRUCTION_OSR_BITS){
					OSR(&registers);
				}
				if((current_instruction & MICRO_INSTRUCTION_HLT_BITS) == MICRO_INSTRUCTION_HLT_BITS){
					HLT();
				}
				break;
			}
			break;
		}
		registers.PC++;	// increment the PC

		#ifdef MEMORY_DEBUG
			printf("Opcode: %u Addressing mode: %u\n", registers.IR, addressing_mode);
		#endif

		if(addressing_mode == DIRECT_MODE) {
			clock_cycles += opcode_cycles[registers.IR];
		}
		else if(addressing_mode == INDIRECT_MODE) {
			clock_cycles += opcode_cycles[registers.IR] + 1;
		}
		else {	/* Autoincrement mode */
			clock_cycles += opcode_cycles[registers.IR] + 2;
		}

		#ifdef DEBUG
			printf("AC: 0x%X, MB: 0x%X, PC: %o, CPMA: %o\n\n", registers.AC & CUTOFF_MASK, registers.MB & CUTOFF_MASK, registers.PC, registers.CPMA);
		#endif

	} while ((current_instruction & CUTOFF_MASK) != 
				(OP_CODE_MICRO | MICRO_INSTRUCTION_GROUP_BIT | MICRO_INSTRUCTION_HLT_BITS));  // run until halt

} // end run_program

void init_system(int argc, char* argv[]) {
	int i;

	mem_init();
	fill_memory(argc, argv);
	mem_print_valid();

	for(i = 0; i < OPCODE_NUM; i++) {
		opcode_freq[i] = 0;
	}
}

/******************************************************************************
** FILLS MEMORY WITH INPUTTED MEMORY FILE
@CHANGELOG:
	JSP:
	-Modified argv count and first 'if' statement to take tracefile arg
	-Added tracefile generation functions
******************************************************************************/
void fill_memory(int argc, char* argv[]) {
	const uint8_t data_mask = 0x3F;		/* Keep lower 6 bits of the upper/lower bytes */
	const uint8_t address_mask = 0x40;	/* Mask to check the address bit */
	const uint8_t high_shift	= 6;		/* Shifts the high byte */
	FILE* program_file;
	uint16_t high_byte, low_byte, word_value;
	static uint16_t address = 0;

	int return1, return2;
	return1 = return2 = 0;

	if(argc == 3) {
		program_file = fopen(argv[1], "r");
		trace_name = argv[2];
	}
	else {
		printf("Please provide a valid program filename to use with the simulator.\n");
		printf("FORMAT:\n PDP_sim <input_file> <trace_file>\n");
		exit(-1);
	}
	if(program_file == NULL) {
		printf("The entered program filename does not exist.\n");
		exit(-2);
	}

	/* Main loop that loads the memory */
	for(;;) {
		/* Grab up to a maximum of three octal values */
		return1 = fscanf(program_file, "%3" SCNo16, &high_byte);
		return2 = fscanf(program_file, "%3" SCNo16, &low_byte);
	
		if(return1 == EOF || return2 == EOF)
			break;

		word_value = ((high_byte & data_mask) << high_shift) | (low_byte & data_mask);

		#ifdef FILL_DEBUG
			printf("From file - high: %o, low: %o\n", high_byte, low_byte);
			printf("Word_value: %o\n", word_value);
		#endif

		if(high_byte & address_mask) {
			address = word_value;
			#ifdef FILL_DEBUG
				printf("Address changed to: %o\n", address);
			#endif
		}
		else {		
			memory[address] = word_value | MEMORY_VALID_BIT;
			#ifdef FILL_DEBUG
				printf("Memory value at address %o set to: %o\n", address, memory[address]);
			#endif
			address++;
		}

		#ifdef FILL_DEBUG
			printf("\n");
		#endif
	}/*end for*/

	fclose(program_file);
}

void print_stats(void) {
	int i;
	uint32_t executed_total = 0;

	printf("*************************************************************\n");
	printf("***********************PRINTING STATS************************\n");
	printf("*************************************************************\n");
	printf("Total clock cycles = %u\n", clock_cycles);
	
	for(i = 0; i < OPCODE_NUM; i++) {
		printf("Opcode number %d was executed %u times\n", i, opcode_freq[i]);
		executed_total += opcode_freq[i];
	}

	printf("The total number of opcodes executed = %u\n", executed_total);
}

/******************************************************************************
**	EOF
******************************************************************************/
