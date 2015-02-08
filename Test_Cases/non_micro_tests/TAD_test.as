/ Date : Feb 7, 2015
/
/ Desc : Tests the TAD instruction
/-------------------------------------------
/ Code Section
*0200			/ start at address 0200
Main,	
	tad A					/ 5 in accum
	tad D					/ 15 in accum = 17 octal
	tad B					/ -5 in accum = 7773 octal
	and zero				/ accum = 0
	tad C					/ accum = 3777 octal
	tad D					/ accum = 
	hlt					/ Halt the system
	jmp Main				/Incase something goes wrong
		
/
/ Data Section
/
*0250 					/ place data at address 0250
A, 5						/ 5
B, 7761					/-15 in twos compliment
C, 3777					/ 2047 largest positive number
D, 12						/ 10
zero, 0

$Main 			/ End of Program; Main is entry point
