/******************************************************************************
** ECE486/586 PDP-8 Simulator
** Sean Koppenhafer, Luis Santiago, Ken, J.S. Peirce
** 
** 21 JANUARY 2015
** MEMORY.C 	IMPLEMENTATION FILE FOR MEMORY OP FUNCTIONS
******************************************************************************/
#include "memory.h"
/******************************************************************************
** 	READ FROM MEMORY
******************************************************************************/
uint16_t mem_read(uint16_t to_convert){
	uint8_t page;
	uint8_t offset;
	uint16_t converted;
	//parse address from CPMA
	page = (0b0000111110000000 & to_convert);//0xF80
	offset = (0b0000000001111111 & to_convert);//0x7F
	converted = (page + offset);
	#ifdef DEBUG
		printf("UNCONVERTED:\n");
		printf("Address:\n\tHEX: 0x%x\tOCTAL: %o\tDEC: %lu\n",to_convert,to_convert,to_convert);
		printf("CONVERTED:\n");
		printf("Address:\n\tHEX: 0x%x\tOCTAL: %o\tDEC: %lu\n",converted,converted,converted);
		printf("Page:\n\tHEX: 0x%x\tOCTAL: %o\tDEC: %lu\n",page,page,page);
		printf("Offset:\n\tHEX: 0x%x\tOCTAL: %o\tDEC: %lu\n",offset,offset,offset);
	#endif
	//access memory at address in array
	//place read data in MB
	return memory[converted];
}/*end mem_read()*/

/******************************************************************************
** 	WRITE TO MEMORY 	
******************************************************************************/
void mem_write(uint16_t to_convert, uint16_t data){
	uint8_t page;
	uint8_t offset;
	uint16_t converted;
	//parse address from CPMA
	page = (0b0000111110000000 & to_convert);//0xF80
	offset = (0b0000000001111111 & to_convert);//0x7F
	converted = (page + offset);
	#ifdef DEBUG
		printf("UNCONVERTED:\n");
		printf("Address:\n\tHEX: 0x%x\tOCTAL: %o\tDEC: %lu\n",to_convert,to_convert,to_convert);
		printf("CONVERTED:\n");
		printf("Address:\n\tHEX: 0x%x\tOCTAL: %o\tDEC: %lu\n",converted,converted,converted);
		printf("Page:\n\tHEX: 0x%x\tOCTAL: %o\tDEC: %lu\n",page,page,page);
		printf("Offset:\n\tHEX: 0x%x\tOCTAL: %o\tDEC: %lu\n",offset,offset,offset);
	#endif

	//go to location in memory array
	memory[converted] = data;

	#ifdef DEBUG
		printf("CALLEE->WROTE: %o to: %o IN OCTAL\n", data, converted);
		printf("CALLEE->WROTE: %lu to: %lu IN UNSIGNED DEC\n", data, converted);
	#endif
}/*end mem_write*/



/******************************************************************************
 * 	EOF
 *****************************************************************************/
