//// The Deep♂Dark♂Fantasy
//
//#include "stepper.h"
//
////initialize variables
//stepper &stepper::begin() {
//    pinMode(dirPin, OUTPUT);
//    pinMode(stepPin, OUTPUT);
//    pinMode(leftBondPin, INPUT);
//    pinMode(rightBondPin, INPUT);
//    pos = 0;
//    bondTriggered = 0;
//    return *this;
//}
//
//// override current position
//stepper &stepper::setPos(const int aPos) {
//    pos = aPos;
//    return *this;
//}
//
//bool stepper::calibrate() {
//    step(STEPPER_LEFT);
//
//}
//
//// move one step, dir = 0: left, dir = 1: right
//stepper &stepper::step(const int dir) {
//    int leftReading = digitalRead(leftBondPin);
//    int rightReading = digitalRead(rightBondPin);
//    if (leftReading || rightReading) {
//        bondTriggered = 2 * leftReading + rightReading;
//        return *this;
//    }
//
//    digitalWrite(dirPin, dir);
//    digitalWrite(stepPin, HIGH);
//    delayMicroseconds(500);
//    digitalWrite(stepPin, LOW);
//    delayMicroseconds(500);
//
//    pos += (dir * 2 - 1);
//    return *this;
//}
//
//////not finished
////int stepper::checkBond() {
////    return bondTriggered;
////}
////
//////not finished
////void stepper::stepCycle(const int dir, const int cycleNum) {
////    step(dir, full_cycle * cycleNum);             //default: 200 step/cycle
////}
//
//int stepper::getPos() {
//    return pos;
//}
//
//
