/******************************************************************************
** ECE486/586 PDP-8 Simulator
** Sean Koppenhafer, Luis Santiago, Ken
**
** main.c
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "memory.h"
#include "cpu.h"

uint8_t fill_memory(int argc, char* argv[]);

int main(int argc, char* argv[]) {
	
	fill_memory(argc, argv);
}	


uint8_t fill_memory(int argc, char* argv[]) {
	FILE* program_file;
	size_t file_size;
	struct stat stats;
	int current_byte;

	if(argc > 1 && argc < 3) {
		program_file = fopen(argv[1], "r");
	}
	else {
		printf("Please provide a valid program filename to use with the simulator.\n");
		exit(-1);
	}

	if(program_file == NULL) {
		printf("The entered program filename does not exist.\n");
		exit(-2);
	}

	/* Get the length of the program in bytes */
	if(stat(argv[1], &stats) == 0) {
		file_size = (size_t)stats.st_size;
	}
	else {
		printf("Could not determine the size of the inputted file\n");
	}

	/* Words are only 12 bits long, but the first two bits of each byte
	** are passed by our special assembler.  Making our memory actually
	** use 16 bit words
	*/


}

