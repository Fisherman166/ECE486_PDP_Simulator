// debugger.c

#include <stdio.h>
#include "debugger.h"

void debugger_init(int argc, char* argv[]){
	if(argc == 4){
		printf("debugger: = %s\n", argv[4 - 1]);
	}

}
