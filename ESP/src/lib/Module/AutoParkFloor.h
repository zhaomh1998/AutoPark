//
// Created by Scott Zhao on 2019-03-03.
//

#ifndef ESP_AUTOPARKFLOOR_H
#define ESP_AUTOPARKFLOOR_H
#define PARKING_SENSOR 16
#define CART_SENSOR 13
#define CART_LEFT_LIMIT_SWITCH 12
#define CART_RIGHT_LIMIT_SWITCH 3
#define ELEVATOR_POS 0
#define LOT1_POS 1
#define LOT2_POS 2
#define LOT3_POS 3

#include "lib/Communication/slave.h"

struct CarCommand {
    CarCommand(uint8_t aCarIndex, uint8_t lotIndex, bool inOrOut): carIndex(aCarIndex) {
        if(inOrOut) {
            startingPos = ELEVATOR_POS;
            // TODO: how to calc lot position
        }
    }
    uint8_t carIndex;
    uint8_t startingPos;
    uint8_t endingPos;
};

class AutoParkFloor : public slave{
public:
    AutoParkFloor(uint8_t floorIndex, bool debugMode);

    bool messageHandler() override {  // override default ESPNow message handler

    }
    // Calibration
    void calibrateCart();
    void calibrateSensor();

    // Stepper
    void step(bool direction);
    bool beSafe();  // Catch any unexpected switch trigger

    // Automatic
    void carBackUp(uint8_t carIndex);
    void carEnterElevator(uint8_t carIndex);
    void carEnterLot(uint8_t carIndex);

    // Communication
    void carForward(uint8_t carIndex);
    void carBackward(uint8_t carIndex);
};


#endif //ESP_AUTOPARKFLOOR_H
