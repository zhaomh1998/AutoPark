// The Deep♂Dark♂Fantasy

#include "stepper.h"

//initialize variables
stepper& stepper::begin(const int dirPin, const int stepPin, const int leftBondPin,
 const int rightBondPin, const int aFull_cycle){
    pinMode(dirPin,OUTPUT);      pinMode(stepPin,OUTPUT);
    pinMode(leftBondPin,INPUT);  pinMode(rightBondPin,INPUT);
    full_cycle = aFull_cycle;
    pos = 0;
    return*this;
}

//overide current pos
stepper& stepper::setPos(const int aPos){
    pos = aPos;
    return*this;
}

stepper& stepper::tune(){
    while((digitalRead(leftBondPin) == 0) || (digitalRead(rightBondPin) == 0)){
        
    }
}
// move one step, dir = 0: left, dir = 1: right
stepper& stepper::step(const int dir){ 
    if((digitalRead(leftBondPin) == 1) || (digitalRead(rightBondPin) == 1)){
        bondTriggered = 2*(int)digitalRead(leftBondPin) + (int)digitalRead(rightBondPin);
        return*this;
    }
    
    digitalWrite(dirPin,dir);
    digitalWrite(stepPin,HIGH);
    delayMicroseconds(500);
    digitalWrite(stepPin,LOW);
    delayMicroseconds(500);
    
    pos += (dir * 2 - 1);
    return*this;
}
//not finished
int stepper::checkBond(){
        return bondTriggered;
}
//not finished
void stepper::stepCycle(const int dir, const int cycleNum){
    step(dir, full_cycle * cycleNum);             //default: 200 step/cycle
}

int stepper::getPos(){
    return pos;
}
    

