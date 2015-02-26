// debugger.c

#include <stdio.h>
#include "debugger.h"

#define DEBUGGER_ARG_POSITION 4

typedef enum debugger_tag {OFF, PAUSE, STEP, NEXT, RUN} DEBUGGER_STATE;

// local globals
static DEBUGGER_STATE state = OFF;

void debugger_init(int argc, char* argv[]){
	if(argc == DEBUGGER_ARG_POSITION && argv[DEBUGGER_ARG_POSITION - 1][0] == 'd'){
		printf("debugger: = %s\n", argv[DEBUGGER_ARG_POSITION - 1]);
		state = PAUSE;
	}

}

void debugger_post_program_run(void){
	printf("shutting down debugger\n");
	state = OFF;
}

void debugger_pre_instruction_fetch(regs *registers){
	char s[80];

	printf("PC: %o, enter to continue: ", registers->PC);
	gets(s);
}

unsigned int debugger_running(void){
	if(state == OFF){
		return(0);
	} else {
		return(1);
	}
}
