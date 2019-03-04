//
// Created by Scott Zhao on 2019-03-03.
//

#ifndef ESP_AUTOPARKFLOOR_H
#define ESP_AUTOPARKFLOOR_H
#define CART_PHOTORESISTOR_1 12
#define LOT_PHOTORESISTOR_2 13
#define CART_LS_R 14
#define CART_LS_L 16

#define ELEVATOR_POS 0
#define LOT0_POS 1
#define LOT1_POS 2
#define LOT2_POS 3

#include "lib/Communication/slave.h"

struct CarCommand {
    CarCommand(uint8_t aCarIndex, uint8_t lotIndex, bool inOrOut): carIndex(aCarIndex) {
        uint16_t targetLotPosition = ELEVATOR_POS;
        switch(lotIndex){
            case 0: targetLotPosition = LOT0_POS; break;
            case 1: targetLotPosition = LOT1_POS; break;
            case 2: targetLotPosition = LOT2_POS; break;
        }
        if(inOrOut) {
            startingPos = ELEVATOR_POS;
            endingPos = targetLotPosition;
        }
        else {
            startingPos = targetLotPosition;
            endingPos = ELEVATOR_POS;
        }
    }

    uint8_t carIndex;
    uint16_t startingPos;
    uint16_t endingPos;
};

class AutoParkFloor : public slave{
public:
    AutoParkFloor(uint8_t floorIndex, uint8_t masterIndex, bool debugMode);

//    bool messageHandler() override;  // override default ESPNow message handler

//    // Calibration
//    bool calibrateCart();
//    bool calibrateSensor();

    // Sensor
    bool getCartLeftLS() {       // Return if the cart left limit switch is pressed
        return digitalRead(CART_LS_L);
    }
//    bool getCartRightLS();      // Return if the cart right limit switch is pressed
//    bool getCartLaser();        // Return if the cart laser is blocked
//    bool getLotLaser();         // Return if the lot laser is blocked
//    bool getElevatorLaser();    // Communicate with 1st floor to get elevator laser (warning: network delay will happen)
//
//    // Stepper
//    void step(bool direction);
//    bool beSafe();  // Catch any unexpected switch trigger

    // Automatic
    void carBackUp(uint8_t carIndex){
        unsigned long timeStart = millis();
        uint8_t backward[1] = {0x01};
        uint8_t shortBreak[1] = {0x04};
        Serial.println("LS is");
        Serial.println(getCartLeftLS());
        ESPNow::send(macs[CAR1], backward, 1);
        while(getCartLeftLS()) {
            yield();
            if(millis() - timeStart > 500) {
                timeStart = millis();
                ESPNow::send(macs[CAR1], backward, 1);
            }
        }
        ESPNow::send(macs[CAR1], shortBreak, 1);
    }
//    void carEnterElevator(uint8_t carIndex);
//    void carEnterLot(uint8_t carIndex);
//
//    // Communication
    bool sendCarCommand(uint8_t carIndex, uint8_t *commandToSend, uint16_t noResponseTimeout = 500) {  // Send the command to a car, and wait for an ack to make sure the command went through
        ESPNow::send(macs[carIndex], commandToSend, 1);

        // TODO
        return true;
    }
//    void carForward(uint8_t carIndex);
//    void carBackward(uint8_t carIndex);

//private:
//    uint16_t stepperPosition;
};


#endif //ESP_AUTOPARKFLOOR_H
