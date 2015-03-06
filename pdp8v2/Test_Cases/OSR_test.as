/ Program : OSR_test.as
/ Date : March 5th, 2015
/
/ Desc : This program tests the OSR instruction
/ Should take 5 cycles and 4 instructions
/
/-------------------------------------------
/
/ Code Section
/
*0200			/ start at address 0200
Main, 	cla cll 	/ clear AC and Link
	OSR		/ Grab switch register value
	dca switch 	/ store SR value at switch
	hlt 		/ Halt program
/
/ Data Section
/
*0250 			/ place data at address 0250
switch,	0		/ Holds switch reigster value
$Main 			/ End of Program; Main is entry point
