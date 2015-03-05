/******************************************************************************
** ECE486/586 PDP-8 Simulator
** Sean Koppenhafer, Luis Santiago, Ken Benderly, J.S. Peirce
** 
** 21 JANUARY 2015
** MEMORY.C 	IMPLEMENTATION FILE FOR MEMORY OP FUNCTIONS
******************************************************************************/
#include "memory.h"
#include "cpu.h"

uint8_t tracepoint_reached;
uint8_t tracepoint_number;
extern regs* registers;

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
		#ifdef PRETTY_PRINT
		fprintf( trace_file, "DR %04o\n", converted);
		#else
		fprintf( trace_file, "0 %04o\n", converted);
		#endif
	}
	else if(read_or_fetch == INSTRUCTION_FETCH) {
		retval = memory[converted];
		#ifdef PRETTY_PRINT
		fprintf( trace_file, "IF %04o\n", converted);
		#else
		fprintf( trace_file, "2 %04o\n", converted);
		#endif
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
	#ifdef PRETTY_PRINT
	fprintf( trace_file, "DW %04o\n", converted);
	#else
	fprintf( trace_file, "1 %04o\n", converted);
	#endif
		
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
			printf("%04o       %04o\n", i, memory[i] & MEMORY_MASK);
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
** WRITES TO THE MEMORY TRACE FILE IN THE MIDDLE OF THE PROGRAM
******************************************************************************/
void update_memory_trace(void) {
	fclose(trace_file);
	trace_file = fopen(trace_name, "a");
		
	#ifdef TRACE_DEBUG
	if(trace_file == NULL) {
		printf("ERROR: Failed to open memory trace file for append.\n");
	}
	else {
		printf("Branch trace file opened successfully for append\n");
	}
	#endif
}

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
**	RETURNS 0 FOR DIRECT ADDRESSING page 0
**              1 FOR DIRECT ADDRESSING CURRENT PAGE
**              2 FOR INDIRECT ADDRESSING PAGE O 
**              3 FOR INDIRECT CURRENT PAGE,
**              4 FOR AUTOINCREMENT
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
**	PRINTS A PAGE OF MEMORY TO A TEXT FILE NAMED MEMORY_PAGE.TXT
** CAN BE OPENED BY THE GUI TO PRINT TO THE TEXT WINDOW
******************************************************************************/
void print_memory_page(int page_number) {
	uint16_t page_base_address, page_max_address, index;
	uint16_t data_values[8];
	FILE* memory_print_file;
	char* memory_print_file_name = "MEMORY_PAGE.txt";
#ifdef USE_LIST_FILE
	unsigned int i;
#endif
	
	memory_print_file = fopen(memory_print_file_name, "w");
	
	if(memory_print_file == NULL) {
		printf("Error opening MEMORY_PAGE.txt\n");
	}
	else {
#ifdef USE_LIST_FILE
		for(i=0; i < PAGES * WORDS_PER_PAGE; i++){
			if(memory[i] & MEMORY_VALID_BIT){
				// first print breakpoint, tracepoint bits
				if(memory[i] & MEMORY_BREAKPOINT_BIT){
					fprintf(memory_print_file, "b");
				} else {
					fprintf(memory_print_file, "   ");
				}
				if(memory[i] & MEMORY_TRACEPOINT_BIT){
					fprintf(memory_print_file, "t ");
				} else {
					fprintf(memory_print_file, "   ");
				}
				// print current PC indicator, or a bunch of spaces
				if(i == registers->PC){
					fprintf(memory_print_file, "==> ");
				} else {
					fprintf(memory_print_file, "         ");
				}
				
				fprintf(memory_print_file, "%04o %04o\n", i, memory[i] & MEMORY_MASK);
			}
		}
#else
		//Move the 5 bit page number to the page bits a PDP8 address
		page_base_address = (page_number & 0x1F) << 7;
		page_max_address = ((page_number + 1) & 0x3F) << 7;
		
		fprintf(memory_print_file, "    \t|            LSB OCTAL DIGIT OF ADDRESS             |\n");
		fprintf(memory_print_file, "    \t|-----------------------------------------------------------------------|\n");
		fprintf(memory_print_file, "    \t|   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |\n");
		fprintf(memory_print_file, "MSB  |-----------------------------------------------------------------------|\n");
		for(index = page_base_address; index < page_max_address; index += 8) {
			//Grab the data values from memory
			data_values[0] = memory[index] & MEMORY_MASK;
			data_values[1] = memory[index + 1] & MEMORY_MASK;
			data_values[2] = memory[index + 2] & MEMORY_MASK;
			data_values[3] = memory[index + 3] & MEMORY_MASK;
			data_values[4] = memory[index + 4] & MEMORY_MASK;
			data_values[5] = memory[index + 5] & MEMORY_MASK;
			data_values[6] = memory[index + 6] & MEMORY_MASK;
			data_values[7] = memory[index + 7] & MEMORY_MASK;
			
			fprintf(memory_print_file, "%03o | %04o %04o %04o %04o %04o %04o %04o %04o|\n", 
					(index & 07770) >> 3, data_values[0], data_values[1], data_values[2], 
					data_values[3], data_values[4], data_values[5], data_values[6], 
					data_values[7]);
		}
		fprintf(memory_print_file, "       |------------------------------------------------------------------------|\n");
#endif
	}

	fclose(memory_print_file);
}

/******************************************************************************
 * 	EOF
 *****************************************************************************/
