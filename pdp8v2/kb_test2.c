//dev/input/event2

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
//#include <string.h>
#include "main.h"
#include <errno.h>
#include <fcntl.h>
#include <dirent.h>
#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <sys/time.h>
#include <termios.h>
#include <signal.h>
#include <linux/kd.h>

int main(int argc, char* argv[])
{
    struct input_event ev[1];
    int fevdev = 0;
    int result = 0;
    int size = sizeof(struct input_event);
   // int rd;
    int value;
    char name[256] = "Unknown";
    char *device = "/dev/input/event2";
    char inputted_char;
    uint8_t test;
//    int kb_request= 0;

    fevdev = open(device, O_RDONLY, O_NONBLOCK );
    if (fevdev == -1) {
        printf("Failed to open event device.\n");
        exit(1);
    }

    result = ioctl(fevdev, EVIOCGNAME(sizeof(name)), name);
    printf ("Reading From : %s (%s)\n", device, name);

  
   //   printf ( "enter kb value: \n");
   //   scanf( "%d", & kb_request);
    
      while (1)
      {
        if ((read(fevdev, ev, size * 64)) < size) {
            break;
        }

        value = ev[0].value;

        if (value != ' ' && ev[1].value == 1 && ev[1].type == EV_KEY) {
            
	    printf ("\t code: %d ", ev[1].code);
            
            result = ioctl(fevdev, EVIOCGKEY(sizeof(test)), &test);
        
 	    printf ("\t type: %d \t result %d \n", test,result);
        }
 
     }
    printf("Exiting.\n");
    close(fevdev);
    return 0;
}
