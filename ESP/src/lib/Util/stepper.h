//#ifndef STEPPER_H
//#define STEPPER_H
//#define FULL_CYCLE 200
//#define STEPPER_LEFT 0
//#define STEPPER_RIGHT 1
//#define STEPPER_FULL_CYCLE 200
//
//#include <Arduino.h>
//#include "logger.h"
//
//class stepper {
//public:
//    stepper(const int dirPin, const int stepPin, const int leftBondPin,
//            const int rightBondPin) : dirPin(dirPin), stepPin(stepPin),
//                                      leftBondPin(leftBondPin),
//                                      rightBondPin(rightBondPin) {}
//
//    ~stepper() = default;
//
//    stepper &begin();
//
//    bool calibrate(); // Go one step and return if reaches the end
//    stepper &step(const int dir)
//
//    stepper &setPos(const int aPos); // override current position with given value
//
//    int getPos();
//
//
//protected:
//    logger logHandle;
//    int pos;
//    int dirPin;
//    int stepPin;
//    int leftBondPin;
//    int rightBondPin;
//    int bondTriggered; //0 = not triggered, 1 = right, 2 = left
//};
//
//#endif