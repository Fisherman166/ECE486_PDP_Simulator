*10
Ptr,	Array-1


*50 
D,	300

*60
indirect,  250

*0200			/ start at address 0200
Main, 	cla cll 	/ clear AC and Link
	tad i Ptr 	/ Auto inc. add Array to Accumulator
	dca  A          / Current page Direct 
	tad  A		 /only a in acc
	dca i  Ptr2Array / Current page indirect
	tad i Ptr        / Auto Inc.
	dca  B 		 / Current page direct
	tad  B           / 
        dca z i D       / Zero page indirect
        dca z D
	tad i Ptr
	dca C 		/ store sum at C
	hlt 		/ Halt program
	jmp Main	/ To continue - goto Main
/
/ Data Section
/

*240
ptr2Array,   300

*0250 			/ place data at address 0250
A, 	0 		/ A equals 1
B, 	0 		/ B equals 2
C, 	0		/ C equal  3

/ auto incremented
*300
Array,	1
	2
	3
	4
	0



$Main 			/ End of Program; Main is entry point
