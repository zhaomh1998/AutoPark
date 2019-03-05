//
// Created by Scott Zhao on 2019-03-03.
//

#ifndef ESP_AUTOPARKFLOOR_H
#define ESP_AUTOPARKFLOOR_H
#ifndef ICACHE_FLASH
#define ICACHE_RAM_ATTR
#endif

// Sensors
#define CART_PHOTORESISTOR_1 12
#define LOT_PHOTORESISTOR_2 13
#define CART_LS_R 14
#define CART_LS_L 16

// Stepper
#define STEPPER_DIR_PIN 5
#define STEPPER_STEP_PIN 4
#define STEPPER_LEFT 0
#define STEPPER_RIGHT 1
#define ELEVATOR_POS 0
#define LOT1_POS 1
#define LOT2_POS 2
#define LOT3_POS 3

#include "lib/Communication/slave.h"
#include <Ticker.h>  // Timer library for driving stepper

enum class FloorStatus {notCalibrated, ready, working, error};

class AutoParkFloor : public slave{
public:
    AutoParkFloor(uint8_t floorIndex, uint8_t masterIndex, bool debugMode);

    bool messageHandler() override;  // override default ESPNow message handler

//    // Calibration
//    bool calibrateCart();
//    bool calibrateSensor();

    // Sensor
    bool getCartLeftLS() {       // Return if the cart left limit switch is pressed
        return (bool) digitalRead(CART_LS_L);
    }
    bool getCartRightLS() {      // Return if the cart right limit switch is pressed - used to emergency stop stepper
        return (bool) digitalRead(CART_LS_R);
    }
//    bool getCartLaser();        // Return if the cart laser is blocked
//    bool getLotLaser();         // Return if the lot laser is blocked
//    bool getElevatorLaser();    // Communicate with 1st floor to get elevator laser (warning: network delay will happen)
//
    // Stepper
    static int16_t targetStepCount;
    static int16_t currentStepCount;
    static Ticker stepperCallbacker;

    static void ICACHE_RAM_ATTR step() {
        digitalWrite(STEPPER_STEP_PIN, HIGH);
        delayMicroseconds(500);
        digitalWrite(STEPPER_STEP_PIN, LOW);
        delayMicroseconds(500);
        targetStepCount++;
        if(targetStepCount >= currentStepCount)
            stepperCallbacker.detach();
    }

    bool moveCartTo(int16_t aPos) {
        if(!debugAssert(myStatus  != FloorStatus::notCalibrated,
                "Error 209 Attempting to move cart before calibration!")) return false;
        if(aPos > stepperPosition)
            digitalWrite(STEPPER_DIR_PIN, STEPPER_LEFT);
        else if(aPos < stepperPosition)
            digitalWrite(STEPPER_DIR_PIN, STEPPER_RIGHT);
        else
            return true;

        targetStepCount = abs(aPos - stepperPosition);
        currentStepCount = 0;
        stepperCallbacker.attach(0.01, step);
        while(currentStepCount != targetStepCount) {
            yield();
            // TODO: Impelment sub-procedure that allows a bigger delay? Implement timeout?
        }
        return true;
    }

    bool calibrateCart();

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
    void carEnterElevator(uint8_t carIndex);
    void carEnterLot(uint8_t carIndex);
//
//    // Communication
    bool sendCarCommand(uint8_t carIndex, uint8_t *commandToSend, uint16_t noResponseTimeout = 500) {  // Send the command to a car, and wait for an ack to make sure the command went through
        ESPNow::send(macs[carIndex], commandToSend, 1);

        // TODO
        return true;
    }
    bool masterMessageHandler();
    bool carMessageHandler(int carIndex);

    void reportStatus() {  // Response for status update request
        switch(myStatus) {
            case FloorStatus::ready: send(FloorCommand(FloorOperation::statusUpdate, FloorArg2::ready), 3); break;
            case FloorStatus::working: send(FloorCommand(FloorOperation::statusUpdate, FloorArg2::working), 3); break;
            case FloorStatus::error: send(FloorCommand(FloorOperation::statusUpdate, FloorArg2::error), 3); break;
            case FloorStatus::notCalibrated: send(FloorCommand(FloorOperation::statusUpdate, FloorArg2::error), 3); break;
        }
    }
//    void carForward(uint8_t carIndex);
//    void carBackward(uint8_t carIndex);

private:
    int16_t stepperPosition;
    bool carMessage;
    bool carAck;
    FloorStatus myStatus;
};


#endif //ESP_AUTOPARKFLOOR_H
