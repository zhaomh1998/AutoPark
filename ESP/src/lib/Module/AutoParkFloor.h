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
#define CALIB_BACKUP_DISTANCE 100
#define ELEVATOR_POS 0
#define LOT1_POS 615
#define LOT2_POS 1135
#define LOT3_POS 1665

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
        send(FloorCommand(FloorOperation::statusUpdate, FloorArg2::getElevatorLaser), MSG_LEN);
            // Waiting for response
        while (!messagePending) { yield(); }
        if (whoIsThisIndex(messageOrigin) != FLOOR1) {debugSendLn("Error 210 Wrong message origin"); return false;}
        if (messageData[0] != TARGET_ACK) {debugSendLn("Error 211 Wrong message target"); return false;}
        return messageData[1] == ACK_TRUE;
    }

    // Stepper
    static int16_t stepperPosition;
    static int16_t targetStepCount;
    static int16_t currentStepCount;
    static int stepperDirection;
    static Ticker stepperCallbacker;

    static void ICACHE_RAM_ATTR singleStep() {
        digitalWrite(STEPPER_STEP_PIN, HIGH);
        delayMicroseconds(1000);
        digitalWrite(STEPPER_STEP_PIN, LOW);
        stepperPosition += stepperDirection;
//        if(currentStepCount % 500 == 0) {
//            yield();
//        }
        if(stepperPosition == targetStepCount)
            stepperCallbacker.detach();
    }

    static bool calibFinished;
    static void ICACHE_RAM_ATTR calibrateCartCallback() {
        digitalWrite(STEPPER_STEP_PIN, HIGH);
        delayMicroseconds(1000);
        digitalWrite(STEPPER_STEP_PIN, LOW);
        currentStepCount ++;
//        if(currentStepCount % 500 == 0) {
//            yield();
//        }
        if(digitalRead(CART_LS_L) || currentStepCount >= targetStepCount) {
            stepperCallbacker.detach();
        }
    }

    bool moveCartTo(int16_t aPos) {
        if(!debugAssert(myStatus != FloorStatus::notCalibrated,
                "Error 209 Attempting to move cart before calibration!")) return false;
        uint8_t responsiveLS;
        if(aPos > stepperPosition) {
            digitalWrite(STEPPER_DIR_PIN, STEPPER_RIGHT);
            responsiveLS = CART_LS_R;
            stepperDirection = 1;
        }
        else if(aPos < stepperPosition) {
            digitalWrite(STEPPER_DIR_PIN, STEPPER_LEFT);
            Serial.println("Going left");
            responsiveLS = CART_LS_L;
            stepperDirection = -1;
        }
        else
            return true;

        myStatus = FloorStatus::working;
        targetStepCount = aPos;
        debugSendLn("Stepper calibration started");
        turnOnStepper();
        stepperCallbacker.attach_ms(2, singleStep);
        while(stepperPosition != targetStepCount) {
            if(digitalRead(responsiveLS)) {
                stepperCallbacker.detach();
                debugSendLn("Error 214 Warning! A LS is triggered!");
                break;
            }
            yield();
        }
        debugSendLn("Stepper finished");
        turnOffStepper();
        myStatus = FloorStatus::ready;
        return true;
    }

    bool calibrateCart() {
        digitalWrite(STEPPER_DIR_PIN, STEPPER_LEFT);
        myStatus = FloorStatus::notCalibrated;
        currentStepCount = 0;
        targetStepCount = 200;
        debugSendLn("Stepper calibration started");
        turnOnStepper();
        stepperCallbacker.attach_ms(2, calibrateCartCallback);
        while(!getCartLeftLS()) {
            if(currentStepCount >= targetStepCount && !getCartLeftLS()) {
                Serial.println("Not finished");
                currentStepCount = 0;
                stepperCallbacker.attach_ms(2, calibrateCartCallback);
            }
            yield();
        }
        turnOffStepper();
        // Step 2: back up
        delay(200);
        stepperPosition = 0;
        stepperDirection = 1;
        digitalWrite(STEPPER_DIR_PIN, STEPPER_RIGHT);
        turnOnStepper();
        targetStepCount = CALIB_BACKUP_DISTANCE;

        stepperCallbacker.attach_ms(2, singleStep);
        while(stepperPosition < targetStepCount) {
            yield();
        }

        debugSendLn("Stepper calibrated");
        stepperPosition = 0;
        myStatus = FloorStatus::ready;
        return true;
    }

    void turnOnStepper() {send(FloorCommand(FloorOperation::statusUpdate, FloorArg2::turnOnStepper), MSG_LEN);}
    void turnOffStepper() {send(FloorCommand(FloorOperation::statusUpdate, FloorArg2::turnOffStepper), MSG_LEN);}

    // Automatic
    bool carBackUp(uint8_t carIndex){
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
        return true;
    }

    bool carEnterElevator(uint8_t carIndex) {
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
        return true;
    }

    bool carEnterLot(uint8_t carIndex) {
        unsigned long timeStart = millis();
        debugSendLn("Car Into Lot");
        ESPNow::send(macs[carIndex], CarCmd(CarCommand::forward), MSG_LEN);
        while(!getLotLaser()) {
            yield();
            if(millis() - timeStart > 500) {
                timeStart = millis();
                ESPNow::send(macs[carIndex], CarCmd(CarCommand::forward), MSG_LEN);
            }
        }
        ESPNow::send(macs[carIndex], CarCmd(CarCommand::stop), MSG_LEN);
        delay(500);
        while(getLotLaser()) {
            yield();
            if(millis() - timeStart > 500) {
                timeStart = millis();
                ESPNow::send(macs[carIndex], CarCmd(CarCommand::forward), MSG_LEN);
            }
        }
        ESPNow::send(macs[carIndex], CarCmd(CarCommand::stop), MSG_LEN);
        return true;
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
    FloorStatus myStatus;
    bool carMessage;
    bool carAck;
};


#endif //ESP_AUTOPARKFLOOR_H
