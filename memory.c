/******************************************************************************
** ECE486/586 PDP-8 Simulator
** Sean Koppenhafer, Luis Santiago, Ken Benderly, J.S. Peirce
** 
** 21 JANUARY 2015
** MEMORY.C 	IMPLEMENTATION FILE FOR MEMORY OP FUNCTIONS
******************************************************************************/
#include "memory.h"

/******************************************************************************
** 	READ FROM MEMORY
******************************************************************************/
uint16_t mem_read(uint16_t to_convert){
	uint16_t page;
	uint8_t offset;
	uint16_t converted;
	//parse address from CPMA
	page = (0b0000111110000000 & to_convert);//0xF80
	offset = (0b0000000001111111 & to_convert);//0x7F
	converted = (page + offset);
	#ifdef MEMORY_DEBUG
		printf("UNCONVERTED:\n");
		printf("Address:\n\tHEX: 0x%x\tOCTAL: %o\tDEC: %u\n",to_convert,to_convert,to_convert);
		printf("CONVERTED:\n");
		printf("Address:\n\tHEX: 0x%x\tOCTAL: %o\tDEC: %u\n",converted,converted,converted);
		printf("Page:\n\tHEX: 0x%x\tOCTAL: %o\tDEC: %u\n",page,page,page);
		printf("Offset:\n\tHEX: 0x%x\tOCTAL: %o\tDEC: %u\n",offset,offset,offset);
	#endif
	//access memory at address in array
	//place read data in MB
	return memory[converted];
}/*end mem_read()*/

/******************************************************************************
** 	WRITE TO MEMORY 	
******************************************************************************/
void mem_write(uint16_t to_convert, uint16_t data){
	uint16_t page;
	uint8_t offset;
	uint16_t converted;
	//parse address from CPMA
	page = (0b0000111110000000 & to_convert);//0xF80
	offset = (0b0000000001111111 & to_convert);//0x7F
	converted = (page + offset);
	#ifdef MEMORY_DEBUG
		printf("UNCONVERTED:\n");
		printf("Address:\n\tHEX: 0x%x\tOCTAL: %o\tDEC: %u\n",to_convert,to_convert,to_convert);
		printf("CONVERTED:\n");
		printf("Address:\n\tHEX: 0x%x\tOCTAL: %o\tDEC: %u\n",converted,converted,converted);
		printf("Page:\n\tHEX: 0x%x\tOCTAL: %o\tDEC: %u\n",page,page,page);
		printf("Offset:\n\tHEX: 0x%x\tOCTAL: %o\tDEC: %u\n",offset,offset,offset);
	#endif

	//Make sure to make the location in memory value
	memory[converted] = MEMORY_VALID_BIT | data;

	#ifdef MEMORY_DEBUG
		printf("CALLEE->WROTE: %o to: %o IN OCTAL\n", data, converted);
		printf("CALLEE->WROTE: %u to: %u IN UNSIGNED DEC\n", data, converted);
	#endif
}/*end mem_write*/

/******************************************************************************
** 	INITIALIZE THE MEMORY 	
******************************************************************************/
void mem_init(void){
	unsigned int i;

	for(i=0; i < PAGES * WORDS_PER_PAGE; i++){
		memory[i] &= ~(MEMORY_VALID_BIT | MEMORY_BREAKPOINT_BIT); // clear valid and breakpoint bits
	}	
} // end mem_init

/******************************************************************************
** 	PRINT ALL VALID MEMORY LOCATIONS	
******************************************************************************/
void mem_print_valid(void){
	unsigned int i;

	printf("Valid memory:\n");
	for(i=0; i < PAGES * WORDS_PER_PAGE; i++){
		if (memory[i] & MEMORY_VALID_BIT){
			printf("  %o %o\n", i, memory[i] & MEMORY_MASK);
		}
	}
	printf("\n");
} // end mem_print_valid

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
**	RETURN EFFECTIVE ADDRESS AT ZERO PAGE
******************************************************************************/
uint16_t zeropage (uint16_t instruction)
{
    return instruction & OFFSET_MASK;
}

/******************************************************************************
**	RETURN EFFECTVE ADDRESS AT THE CURRENT PAGE
******************************************************************************/
uint16_t currentpage (uint16_t instruction, regs* reg)
{
    return ( (reg->PC & PAGE_MASK) | (instruction & OFFSET_MASK) );
}

/******************************************************************************
**	DECODE ADDRESS?
******************************************************************************/
uint16_t getaddress(uint16_t instruction,regs* reg)
{
    if (PageMode(instruction))
    {
        return  currentpage(instruction, reg);

    }
    else
    {
        return zeropage(instruction);
    }
}

/******************************************************************************
**	CALCULATE THE EFFECTIVE ADDRESS
******************************************************************************/
void EffAddCalc(uint16_t instruction, regs* reg)
{
    uint16_t tempmem;

    if(AddrMode(instruction))
    {
        /* gets the value of the address to be used
        need read functon to read content and move
        contento to memory register*/

        tempmem = getaddress(instruction, reg);
        // check if address is the rage of auto indexing
        if (tempmem >= 010 && tempmem <= 017)
        {
            // Mem read to the content add one and store back to mem
            // read the content of meme and put back to CPMA
        }
        reg->CPMA= tempmem;
    }
    else
    {
        // not indirect just store the value
        reg->CPMA = getaddress(instruction , reg);
    }
}
/******************************************************************************
 * 	EOF
 *****************************************************************************/
