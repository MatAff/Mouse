
// REMEMBER TO RUN THIS AS SUPER USER (SUDO) TO GET ACCESS TO EVENTS!

// Mouse
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <vector>
#include <iterator>
#include <algorithm>
#include <string>

// HAT
#include "Adafruit_MotorHAT.h"
#include "stdio.h"
//#include <iostream>
#include <signal.h>

//#define MOUSEFILE "/dev/input/event5" // Change based on event
#define MOUSEFILE "/dev/input/event15" // Change based on event

Adafruit_MotorHAT hat;

void ctrl_c_handler(int s){
  std::cout << "Caught signal " << s << std::endl;
  hat.resetAll();
  exit(1); 
}

int main(int argc, char** argv) {

    // Exit
    signal(SIGINT, ctrl_c_handler);

    // HAT
    auto& myStepper = hat.getStepper(2); // # motor port #1
    std::cout << "Got stepper" << std::endl;
    myStepper.setSpeed(30); //  # 30 RPM
 
    // Mouse 
    int fd;
    struct input_event ie;
    int xx, yy;
    std::string dir;
 
    xx = 0;
    yy = 0; 

    // Open mouse connection
    if((fd = open(MOUSEFILE, O_RDONLY)) == -1) {
        printf("Device open ERROR\n");
        exit(EXIT_FAILURE);
    } else {
        printf("Device open OK\n");
    }

    // Print things
    while(read(fd, &ie, sizeof(struct input_event)))
    {
        //unsigned char *ptr = (unsigned char*)&ie;
        //int i;       
        
        // Determine movement
        if(ie.type!=2) { continue; }
	
        //printf("code %d\tvalue %d\n", ie.code, ie.value);
        if (ie.code==1 && ie.value > 0) { dir = "Up"; yy++; }
        if (ie.code==1 && ie.value < 0) { dir = "Down"; yy--; }
        if (ie.code==0 && ie.value > 0) { 
            dir = "Right"; 
            xx++; 
            myStepper.step(100, FORWARD,  SINGLE);
        }
        if (ie.code==0 && ie.value < 0) { 
            dir = "Left"; 
            xx--; 
            myStepper.step(100, BACKWARD, SINGLE);
        }

     }
}
