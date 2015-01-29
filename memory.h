/******************************************************************************
** ECE486/586 PDP-8 Simulator
** Sean Koppenhafer, Luis Santiago, Ken, J.S. Peirce
** 
** 15 JANUARY 2015
** memory.h
@CHANGELOG:
	JSP:
	-added tracefile open/close functions
	-TODO: talk to Luis about args passed for mem read/writes
******************************************************************************/
#define DEBUG 1
#include<inttypes.h>
#include<unistd.h>

#ifndef MEMORY_H
#define MEMORY_H

/* Total memory size is 4K */
#define WORDS_PER_PAGE 128
#define PAGES 32

#define MEMORY_MASK 0xFFF	/*Only 12 bits needed for address or a data*/

uint16_t memory[PAGES * WORDS_PER_PAGE];

uint16_t mem_read(uint16_t to_convert);
void mem_write(uint16_t to_convert, uint16_t data);

char *trace_name;
FILE *trace_file;
/******************************************************************************
**	OPEN THE TRACEFILE TO APPEND, START AT BEGINNING OF FILE: "a+"
******************************************************************************/
int trace_init(){
	int ret_val;
	trace_file = fopen(trace_name, "a+");
	
	if(trace_file == NULL){
	#ifdef TRACE_DEBUG
		printf("ERROR: Unable to open trace_file: %s\n", trace_name);
	#endif
		ret_val = -1;
	}/*end if*/
	else{
	#ifdef TRACE_DEBUG
		printf("trace_file: %s opened successefully\n", trace_name);
	#endif
		ret_val=0;
	}/*end else*/
return ret_val;
}/*end trace_init()*/

/******************************************************************************
** CALL THIS FUNCTION AT COMPLETION OF PROGRAM TO CLOSE TRACEFILE
** TODO: MAY NOT BE A BAD IDEA TO OPEN/CLOSE EACH TIME WE WRITE OUT?
******************************************************************************/
int trace_close(){
	int ret_val;
	ret_val = fclose(trace_file);
return ret_val;
}/*end close_trace()*/

/******************************************************************************
**	EOF
******************************************************************************/
#endif

