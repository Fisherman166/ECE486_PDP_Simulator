/******************************************************************************
** ECE486/586 PDP-8 Simulator
** Sean Koppenhafer, Luis Santiago, Ken Benderly, J.S. Peirce
** 
** 21 JANUARY 2015
** MEMORY.C 	IMPLEMENTATION FILE FOR MEMORY OP FUNCTIONS
******************************************************************************/
#include "memory.h"

uint8_t tracepoint_reached;
uint8_t tracepoint_number;

/******************************************************************************
** 	READ FROM MEMORY
**	FOR READ_OR_FETCH VARIABLE: 0 = DATA READ		1 = INSTRUCTION_FETCH
******************************************************************************/
uint16_t mem_read(uint16_t to_convert, uint8_t read_or_fetch){
	uint16_t page;
	uint8_t offset;
	uint16_t converted;
	uint16_t retval;

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

	//Print to trace file and handle the fetch accordingly
	if(read_or_fetch == DATA_READ) {
		//First check if a tracepoint has been reached
		retval = memory[converted];
		if(retval & MEMORY_TRACEPOINT_BIT) {
			tracepoint_reached = 1;
			tracepoint_number = converted;
		}

		//Remove internal state bits for read
		retval &= CUTOFF_MASK;
		fprintf( trace_file, "DR %04o\n", converted);
	}
	else if(read_or_fetch == INSTRUCTION_FETCH) {
		retval = memory[converted];
		fprintf( trace_file, "IF %04o\n", converted);
	}
	else {
		fprintf( trace_file, "Read type not recognized\n");
	}

	return retval;
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

	/* Write to trace file */
	fprintf( trace_file, "DW %04o\n", converted);
		
	//Check if tracepoint is set
	if(memory[converted] & MEMORY_TRACEPOINT_BIT) tracepoint_reached = 1;

	memory[converted] &= ~MEMORY_MASK;	//Clear the data field
	memory[converted] |= data & MEMORY_MASK;

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

	printf("******************************PRINTING VALID MEMORY**************************\n");
	printf("Address    Contents\n");
	printf("-------    --------\n");
	for(i=0; i < PAGES * WORDS_PER_PAGE; i++){
		if (memory[i] & MEMORY_VALID_BIT){
			printf("%o        %o\n", i, memory[i] & MEMORY_MASK);
		}
	}
	printf("\n");
} // end mem_print_valid

/******************************************************************************
**	OPEN THE TRACEFILE TO APPEND, START AT BEGINNING OF FILE: "a+"
******************************************************************************/
int trace_init(){
	int ret_val;
	trace_file = fopen(trace_name, "w");
	
	if(trace_file == NULL){
	#ifdef TRACE_DEBUG
		printf("ERROR: Unable to open memory trace file: %s\n", trace_name);
	#endif
		ret_val = -1;
	}/*end if*/
	else{
	#ifdef TRACE_DEBUG
		printf("Memory trace file: %s opened successfully\n", trace_name);
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
**	DECODE ADDRESS
******************************************************************************/
// Check bit 4 (on PDP8 )  to federmine current page or zero page mode

uint16_t getaddress(uint16_t instruction,regs* reg, uint8_t * page)
{
	uint16_t retval;
	if (PageMode(instruction)) 
   {
   	retval = currentpage(instruction, reg);
  	*page=1;
   }
   else
   {
   	retval = zeropage(instruction);
	*page=0;
   }

	return retval;
}

/******************************************************************************
**	CALCULATE THE EFFECTIVE ADDRESS
**	RETURNS 0 FOR DIRECT ADDRESSING, 1 FOR INDIRECT ADDRESSING,
** AND 2 FOR AUTOINCREMENT
******************************************************************************/
uint8_t EffAddCalc(uint16_t instruction, regs* reg)
{
	uint16_t ptr_address, indirect_address;
	uint8_t addressing_mode, page_mode;

page_mode =0;
    if(AddrMode(instruction))
    {
        /* Indirect mode
         gets the ponter address*/
	ptr_address = getaddress(instruction, reg, & page_mode);
        indirect_address = mem_read(ptr_address, DATA_READ);
	
	addressing_mode = INDIRECT_MODE+ page_mode;
        
        // check if address is the range of auto indexing
        if (ptr_address >= 010 && ptr_address <= 017)
        {
            addressing_mode = AUTOINCREMENT_MODE;
            ++indirect_address;
            mem_write(ptr_address, indirect_address );
            
        }
        
        reg->CPMA= indirect_address;
    }
    else
    {
        // not indirect just store the value
        reg->CPMA = getaddress(instruction, reg,& page_mode);
		  addressing_mode = DIRECT_MODE + page_mode;
    }

	 return addressing_mode;
}

/******************************************************************************
**	SETS A BREAKPOINT IN MEMORY
******************************************************************************/
void set_breakpoint(uint16_t breakpoint_address) {
	memory[breakpoint_address] |= MEMORY_BREAKPOINT_BIT;
}

/******************************************************************************
**	REMOVES A BREAKPOINT FROM MEMORY
******************************************************************************/
void remove_breakpoint(uint16_t breakpoint_address) {
	memory[breakpoint_address] &= ~MEMORY_BREAKPOINT_BIT;
}

/******************************************************************************
**	SETS A TRACEPOINT IN MEMORY
******************************************************************************/
void set_tracepoint(uint16_t tracepoint_address) {
	memory[tracepoint_address] |= MEMORY_TRACEPOINT_BIT;
}

/******************************************************************************
**	REMOVES A TRACEPOINT FROM MEMORY
******************************************************************************/
void remove_tracepoint(uint16_t tracepoint_address) {
	memory[tracepoint_address] &= ~MEMORY_TRACEPOINT_BIT;
}

/******************************************************************************
 * 	EOF
 *****************************************************************************/
