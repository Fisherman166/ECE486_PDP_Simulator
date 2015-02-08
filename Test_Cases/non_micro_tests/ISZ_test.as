/ Date : Feb 7, 2015
/
/ Desc : Tests the ISZ instruction
/-------------------------------------------
/ Code Section
*0200			/ start at address 0200
Main,
	isz X					/ Increment but don't skip
	isz X					/ Increment but don't skip
	isz X					/ Increment and do skip
	tad Y					/ If this is executed, ISZ is wrong
	hlt					/ Halt the system
	jmp Main				/Incase something goes wrong
		
/
/ Data Section
/
*0250 					/ place data at address 0250
X, 7775			/ -3 decimal
Y, 50				/ 50 octal

$Main 			/ End of Program; Main is entry point
