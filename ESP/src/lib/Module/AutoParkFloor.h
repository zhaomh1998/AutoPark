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
#define ELEVATOR_PHOTORESISTOR_3 3 // For Floor 1 only

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

    bool messageHandler() override;  // override default ESPNow message handler4
    // Calibration
    bool calibrateSensor();

    // Sensor
    bool getCartLeftLS() {return (bool) digitalRead(CART_LS_L);}  // Return if the cart left limit switch is pressed
    bool getCartRightLS() {return (bool) digitalRead(CART_LS_R);} // Return if the cart right limit switch is pressed - used to emergency stop stepper
    bool getCartLaser() {return (bool) digitalRead(CART_PHOTORESISTOR_1);}  // Return if the cart laser is blocked
    bool getLotLaser() {return (bool) digitalRead(LOT_PHOTORESISTOR_2);}    // Return if the lot laser is blocked
    bool getElevatorLaser() {    // Communicate with 1st floor to get elevator laser (warning: network delay will happen)
        if(thisFloor != FLOOR1) {
            ESPNow::send(macs[FLOOR1], FloorCommand(FloorOperation::statusUpdate, FloorArg2::getElevatorLaser), 3);
            while(!messagePending) {yield();}
            if(!debugAssert(whoIsThisIndex(messageOrigin) == FLOOR1, "Error 210 Wrong message origin")) return false;
            if(!debugAssert(messageData[0] == TARGET_ACK, "Error 211 Wrong message target")) return false;
            return messageData[1] == ACK_TRUE;
        }
        else{
            return (bool) digitalRead(ELEVATOR_PHOTORESISTOR_3);
        }
    }

    // Stepper
    static int16_t targetStepCount;
    static int16_t currentStepCount;
    static Ticker stepperCallbacker;

    static void ICACHE_RAM_ATTR singleStep() {
        digitalWrite(STEPPER_STEP_PIN, HIGH);
        delayMicroseconds(500);
        digitalWrite(STEPPER_STEP_PIN, LOW);
        currentStepCount++;
        if(currentStepCount % 500 == 0) {
            yield();
        }
        if(currentStepCount >= targetStepCount)
            stepperCallbacker.detach();
    }
    static void ICACHE_RAM_ATTR calibrateCartCallback() {
        digitalWrite(STEPPER_STEP_PIN, HIGH);
        delayMicroseconds(500);
        digitalWrite(STEPPER_STEP_PIN, LOW);
        currentStepCount++;
//        if(currentStepCount % 500 == 0) {
//            yield();
//        }
        if(digitalRead(CART_LS_L))
            stepperCallbacker.detach();
    }

    bool moveCartTo(int16_t aPos) {
        if(!debugAssert(myStatus  == FloorStatus::notCalibrated,
                "Error 209 Attempting to move cart before calibration!")) return false;
        if(aPos > stepperPosition)
            digitalWrite(STEPPER_DIR_PIN, STEPPER_LEFT);
        else if(aPos < stepperPosition)
            digitalWrite(STEPPER_DIR_PIN, STEPPER_RIGHT);
        else
            return true;

        targetStepCount = abs(aPos - stepperPosition);
        currentStepCount = 0;
        stepperCallbacker.attach_ms(1, singleStep);
        while(currentStepCount != targetStepCount) {
            yield();
            // TODO: Implement sub-procedure that allows a bigger delay? Implement timeout?
        }
        return true;
    }
    bool calibrateCart() {
        digitalWrite(STEPPER_DIR_PIN, STEPPER_LEFT);
        myStatus = FloorStatus::notCalibrated;
        currentStepCount = 0;
        stepperCallbacker.attach_ms(1, calibrateCartCallback);
        while(myStatus != FloorStatus::ready) {
            yield();
            // TODO: Implement sub-procedure that allows a bigger delay? Implement timeout?
        }
        return true;
    }

    // Automatic
    void carBackUp(uint8_t carIndex){
        unsigned long timeStart = millis();
        ESPNow::send(macs[carIndex], CarCmd(CarCommand::backward), MSG_LEN);
        while(!getCartLaser()) {
            yield();
            if(millis() - timeStart > 500) {
                debugSensorSend();
                timeStart = millis();
                ESPNow::send(macs[carIndex], CarCmd(CarCommand::backward), MSG_LEN);
            }
        }
        ESPNow::send(macs[carIndex], CarCmd(CarCommand::stop), MSG_LEN);
    }

    void carEnterElevator(uint8_t carIndex) {
        unsigned long timeStart = millis();
        ESPNow::send(macs[carIndex], CarCmd(CarCommand::forward), MSG_LEN);
        while(!getLotLaser()) {
            yield();
            if(millis() - timeStart > 500) {
                timeStart = millis();
                ESPNow::send(macs[carIndex], CarCmd(CarCommand::forward), MSG_LEN);
            }
        }
        ESPNow::send(macs[carIndex], CarCmd(CarCommand::stop), MSG_LEN);
    }

    void carEnterLot(uint8_t carIndex) {
        unsigned long timeStart = millis();
        debugSendLn("Car back up");
        ESPNow::send(macs[carIndex], CarCmd(CarCommand::forward), MSG_LEN);
        while(!getLotLaser()) {
            yield();
            if(millis() - timeStart > 500) {
                timeStart = millis();
                ESPNow::send(macs[carIndex], CarCmd(CarCommand::forward), MSG_LEN);
            }
        }
        ESPNow::send(macs[carIndex], CarCmd(CarCommand::stop), MSG_LEN);
    }


    bool masterMessageHandler();
    bool carMessageHandler(int carIndex);

    void reportStatus() {  // Response for status update request
        switch(myStatus) {
            case FloorStatus::ready: send(FloorCommand(FloorOperation::statusUpdate, FloorArg2::ready), MSG_LEN); break;
            case FloorStatus::working: send(FloorCommand(FloorOperation::statusUpdate, FloorArg2::working), MSG_LEN); break;
            case FloorStatus::error: send(FloorCommand(FloorOperation::statusUpdate, FloorArg2::error), MSG_LEN); break;
            case FloorStatus::notCalibrated: send(FloorCommand(FloorOperation::statusUpdate, FloorArg2::error), MSG_LEN); break;
        }
    }
    void debugSensorSend() {
        debugSendLn("LS_R:" + (String)getCartRightLS() + "LS_L:" + (String)getCartLeftLS()
        + "PR1_Cart:" + (String)getCartLaser() + "PR2_Lot:" + (String)getLotLaser());
    }
//    void carForward(uint8_t carIndex);
//    void carBackward(uint8_t carIndex);

private:
    uint8_t thisFloor;
    int16_t stepperPosition;
    FloorStatus myStatus;
    bool carMessage;
    bool carAck;
};


#endif //ESP_AUTOPARKFLOOR_H
