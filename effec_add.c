/* Luis D santiago
 *  Addressing modes for PDP-8
 */

#include <inttypes.h>
#include "cpu.h" 	  // needed to access PC
#include <stdio.h>

#define PageMode(var) 	((var) & (1<<7))
#define AddrMode(var) 	((var) & (1<<8))
#define OFFSET_MASK   	0X7F
#define PAGE_MASK	0XF80


uint16_t zeropage (uint16_t instruction)
{
    return instruction & OFFSET_MASK;
}



uint16_t currentpage (uint16_t instruction, regs* reg)
{
    return ( (reg->PC & PAGE_MASK) | (instruction & OFFSET_MASK) );
}



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



void EffAddCalc(uint16_t instruction, regs* reg)
{
    uint16_t tempmem;

    if(AddrMode(instruction))
    {
        /* gets the value of the address to be used
        need read functon to read content and move
        contento to memory register*/

        tempmem=getaddress(instruction , reg);
        // check if address is the rage od auto indexing
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




/* CPU registers - only 12 bits are used of the 16
   typedef struct {
   uint16_t AC;		 Accumulator
   uint16_t MQ;		 Multiplier Quotient
   uint16_t PC;		 Program Counter
   uint16_t MB;		 Memory Buffer
   uint16_t CPMA;	 Central Processor Memory Address
   uint16_t SR;		 Console Switch Register
   uint8_t IR;		 Instruction Register - only 3 bits are used
   uint8_t link_bit;	 Carry out bit
   } regs;
 */


int main ()
{
    regs registers;
    uint16_t instruction;
// once i hace a read and write will test
    EffAddCalc(instruction, &registers);

    return 0;
}

