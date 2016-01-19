/* 
 * File:   main.c
 * Author: Chris Hajduk
 *
 * Created on August 21, 2014, 11:32 PM
 */
 //Edited by Faye for embedded/electronics bootcamp Winter 2016

#include <stdio.h>
#include <stdlib.h>
#include "GPS.h"
#include "main.h"
#include "HunterTruckAPI.h"
#include "debug.h"
#include "timer.h"
#include "OutputCompare.h"
#include "InterchipDMA.h"
#include "PWM.h"

_FOSCSEL(FNOSC_FRC); // Internal FRC oscillator
_FOSC(FCKSM_CSECMD & OSCIOFNC_OFF & POSCMD_NONE);
_FWDT(FWDTEN_OFF & WDTPOST_PS2048 & WDTPRE_PR128); //32,128
/*
 * 
 */

extern GPSData GPS;

void delayTime(int initialTime, int duration){
    while ((getTime() - initialTime)<duration){
        //Do nothing. Incredibly bad but I don't know if the chip has a built in function or not
    }
}

int main(int argc, char** argv) {
    initTruck();
    int initialTime = (int)getTime();
    
    //Delay for 2 seconds
    delayTime(initialTime, 2000);
    
    while(TRUE){
        //This is how you move the car. Throttle goes from -100% to 100%. Steering goes from -100 to 100%.
        setThrottle(0);   //Note that the first -20%/20% is a safety buffer region. Anything less than 20% is equivalent to no throttle.
        setSteering(0);
        
        int i; //The car will keep turning in a square forever
        for(i = 0; i < 4; i++){ //four turns
            setThrottle(40); //assuming car starts out straight
            initialTime = (int)getTime();
            delayTime(initialTime, 3000);
            setSteering(100); //assuming this is turning right, and it'll need 1 second to do a 90 degree turn
            setThrottle(60);
            initialTime = (int)getTime();
            delayTime(initialTime, 1000);
            setSteering(0);
        }
//        This is an example of how you can print the GPS time to the debugging interface.
        //char str[16];
        //sprintf((char *)&str, "Time: %lu", getTime());
        //debug((char *)&str);

        background();
    }
    return (EXIT_SUCCESS);
}

