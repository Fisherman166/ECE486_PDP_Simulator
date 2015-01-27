
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"memory.h"

int main(void){
	char * input;
	uint16_t temp;
	uint16_t ret;
	uint16_t data;
	while(1){	
		data=0;
		printf("Enter a number:\n");
		scanf("%lu", &temp);
		printf("you entered: %lu\n", temp);	
		printf("Temp is: %o as OCTAL\n", temp);
		printf("Temp is: %lu as ULONG\n", temp);	
		printf("Temp is: 0x%x as HEX\n", temp);	
		
		data = (temp+1);
		mem_write(temp, data);
		printf("CALLER->WROTE: %o to %o IN OCTAL\n", data, temp);
		printf("CALLER->WROTE: %lu to %lu IN UNSIGNED DEC\n\n", data, temp);
		ret = mem_read(temp);
		printf("READ RETURNED: %o IN OCTAL\n", ret);
		printf("READ RETURNED: %lu IN UNSIGNED DEC\n\n\n", ret);
	}//end while
return 0;
}
