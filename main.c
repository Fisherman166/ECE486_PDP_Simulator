/******************************************************************************
** ECE486/586 PDP-8 Simulator
** Sean Koppenhafer, Luis Santiago, Ken
**
** main.c
*/

#include "main.h"
#include "memory.h"
#include "cpu.h"


int main(int argc, char* argv[]) {
	
	fill_memory(argc, argv);

	//can capture return values to verify fopen && fclose
	trace_init();
	trace_close();

}/*end main*/

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
			memory[address] = word_value;
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
/******************************************************************************
**	EOF
******************************************************************************/
