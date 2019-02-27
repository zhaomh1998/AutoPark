#ifndef STEPPER_H
#define STEPPER_H
#define FULL_CYCLE 200

#include <Arduino.h>
#include "logger.h"
class stepper {
public:
    stepper(){}
    ~stepper(){}

    stepper& begin(const int dirPin, const int stepPin, const int leftBondPin, 
    const int rightBondPin, const int aFull_cycle = 200);
    stepper& tune();                                           //set pos to left bond
    stepper& step(const int dir);
    stepper& setPos(const int aPos); //overide current position with given value
    void stepCycle(const int dir, const int cycleNum);
    int getPos();    
    int checkBond(); //0 = not triggered, 1 = right, 2 = left
    

    
protected:
    logger logHandle;
    int pos;
    int dirPin;
    int stepPin;
    int full_cycle;
    int leftBondPin;
    int rightBondPin;
    int bondTriggered; //0 = not triggered, 1 = right, 2 = left
    };

#endif