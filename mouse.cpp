
// REMEMBER TO RUN THIS AS SUPER USER (SUDO) TO GET ACCESS TO EVENTS!

#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <vector>
#include <iterator>
#include <algorithm>
#include <string>

//#define MOUSEFILE "/dev/input/event5" // Change based on event
#define MOUSEFILE "/dev/input/event15" // Change based on event

int main()
{
    int fd;
    struct input_event ie;
    int xx, yy;
    std::string dir;
 
    xx = 0;
    yy = 0; 

    // Open connection
    if((fd = open(MOUSEFILE, O_RDONLY)) == -1) {
        printf("Device open ERROR\n");
        exit(EXIT_FAILURE);
    } else {
        printf("Device open OK\n");
    }
    
    // Print things
    while(read(fd, &ie, sizeof(struct input_event)))
    {
        unsigned char *ptr = (unsigned char*)&ie;
        int i;       

        // Display input event
        /*while(read(fd, &ie, sizeof(struct input_event))) {
	    printf("time %ld.%06ld\ttype %d\tcode %d\tvalue %d\n",
	      ie.time.tv_sec, ie.time.tv_usec, ie.type, ie.code, ie.value);
	}*/
        
        // Determine movement
        if(ie.type!=2) { continue; }
	
        //printf("code %d\tvalue %d\n", ie.code, ie.value);
        if (ie.code==1 && ie.value > 0) { dir = "Up"; yy++; }
        if (ie.code==1 && ie.value < 0) { dir = "Down"; yy--; }
        if (ie.code==0 && ie.value > 0) { dir = "Right"; xx++; }
        if (ie.code==0 && ie.value < 0) { dir = "Left"; xx--; }
        
        /*if (ie.code==1)
        {
            if (ie.value > 0) { 
                std::cout << "Down" << std::endl;
                yy++;
            } else {
                std::cout << "Up" << std::endl;
                yy--;
            }
        } else {
            if (ie.value > 0) { 
                std::cout << "Right" << std::endl; 
                xx++;
            } else {
                std::cout << "Left" << std::endl; 
                xx--;
            } 
        }*/

        // Show position (change)
        printf("Position %i\t%i\n", xx,yy);
        //std::cout << dir << std::endl;

    }

    return 0;
}

