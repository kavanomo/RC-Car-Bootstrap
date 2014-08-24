/*
 * File:   PWM.c
 *
 * Created on August 21, 2014, 11:40 PM
 */
#include "PWM.h"
#include "OutputCompare.h"
#include "InputCapture.h"
#include "debug.h"

char initialized = 0;

int pwmArray[NUM_CHANNELS];
float scaleFactorIn[NUM_CHANNELS];
int offsetIn[NUM_CHANNELS];
float scaleFactorOut[NUM_CHANNELS];
int offsetOut[NUM_CHANNELS];

void initPWM(char inputChannels, char outputChannels){
    initIC(inputChannels);
    int i = 0;
    for (i = 0; i < NUM_CHANNELS; i++){
        // Use default scale factor values
        scaleFactorIn[i] = MAX_PWM/(UPPER_PWM - MIDDLE_PWM); // We know that MAX_PWM = (UPPER_PWM - MIDDLE_PWM)/2 for a factor of 1.
        // Use default offset values
        offsetIn[i] = 0;
        // Use default scale factor values
        scaleFactorOut[i] = (UPPER_PWM - MIDDLE_PWM)/MAX_PWM; // We know that MAX_PWM = (UPPER_PWM - MIDDLE_PWM) for a factor of 1.
        // Use default offset values
        offsetOut[i] = 0;
    }
    initOC(outputChannels);
    initialized = 1;
}

void PWMInputCalibration(unsigned int channel, float signalScaleFactor, unsigned int signalOffset){
    if (channel > 0 && channel < NUM_CHANNELS){ //Check if channel number is valid
        scaleFactorIn[channel] = signalScaleFactor;
        offsetIn[channel] = signalOffset;
    }
    else { //Invalid Channel
        //Display Error Message
        error("Invalid PWM channel");
    }
}

void PWMOutputCalibration(unsigned int channel, float signalScaleFactor, unsigned int signalOffset){
    if (channel > 0 && channel < NUM_CHANNELS){ //Check if channel number is valid
        scaleFactorOut[channel] = signalScaleFactor;
        offsetOut[channel] = signalOffset;
    }
    else { //Invalid Channel
        //Display Error Message
        error("Invalid PWM channel");
    }
}


int getPWM(unsigned int channel){
    if (initialized && channel > 0 && channel < NUM_CHANNELS){ //Is the Input Initialized?
        return (getICValue(channel) - offsetIn[channel]) * scaleFactorIn[channel];
    }
    else { //Not initialized or invalid channel
        //Display Error Message
        error("PWM not initialized or invalid channel specified");
        //Return 0
        return 0;
    }
}
int* getPWMArray(){
    if (initialized){ //Is the Input Initialized?
        unsigned int* icArray = getICValues();
        int i = 0;
        for (i = 0; i < NUM_CHANNELS; i++){
            pwmArray[i] = ((int)icArray[i] - offsetIn[i]) * scaleFactorIn[i];
        }
        return pwmArray;
    }
    else { //Not initialized
        //Display Error Message
        error("PWM not initialized");
        //Return 0
        return 0;
    }
}

void setPWM(unsigned int channel, int pwm){
    if (initialized && channel > 0 && channel < NUM_CHANNELS){ //Is the Input Initialized?
        setOCValue(channel, (int)((pwm + offsetIn[channel]) * scaleFactorIn[channel]));
    }
    else { //Not initialized or invalid channel
        //Display Error Message
        error("PWM not initialized or invalid channel specified");
    }
}
void setPWMArray(int* ocArray){
    if (initialized){ //Is the Input Initialized?
        int i = 0;
        for (i = 0; i < NUM_CHANNELS; i++){
            setOCValue(i,(int)((ocArray[i] + offsetIn[i]) * scaleFactorIn[i]));
        }
    }
    else { //Not initialized
        //Display Error Message
        error("PWM not initialized.");
    }
}