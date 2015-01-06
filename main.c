/******************************************************************************
** ECE486/586 PDP-8 Simulator
** Sean Koppenhafer, Luis Santiago, Ken
**
** main.c
*/

#include <stdio.h>
#include "memory.h"
#include "cpu.h"

uint8_t fill_memory(int argc, char* argv[]);

int main(int argc, char* argv[]) {
}	


uint8_t fill_memory(int argc, char* argv[]) {
	FILE* program_file;

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

}
