//
// Created by Scott Zhao on 2019-02-22.
//
#ifndef ESP_CAR_H
#define ESP_CAR_H

#include <Arduino.h>
#include "lib/Communication/slave.h"
#include "lib/Util/RGB.h"
#include "lib/Util/AutoParkConfig.h"

#define MOTOR_A_CONTROL_1 4
#define MOTOR_A_CONTROL_2 5
#define MOTOR_B_CONTROL_1 14
#define MOTOR_B_CONTROL_2 12
#define MOTOR_PWN_PIN 13
//#define CAR_SPEED 200
#define MAX_MOTOR_REFRESH_MS 200
#define MOTOR_AUTO_STOP_MS 1000

class AutoParkCar;

extern AutoParkCar *carInstancePtr;  // Stores this*. For static method to access. Initialized in ctor.

class AutoParkCar : public slave {
public:
    AutoParkCar(uint8_t myCarIndex, bool debugMode);

    bool messageHandler() override {  // overwrite this method in subclasses
        if(!messagePending)
            return false;
        else {
            printESPNowMsg(RECEIVE, messageOrigin, messageData, messageLen);
            if(messageLen == 1) {
                commandDecoder(messageData);
            }
            else{
                log(WARNING, "^^^^^Unexpected ESPNow Message!\n");
            }
            messagePending = false;
            return true;
        }

    }

    void commandDecoder(const uint8_t *data);

    bool allowMotorChange() {
        unsigned long timeNow = millis();
        if (timeNow - motorChangedTime > MAX_MOTOR_REFRESH_MS) {
            motorChangedTime = timeNow;
            return true;
        }
        return false;
    }

    // If stop command get lost, trigger stop
    void autoStop() {
        if(!motorRunning)
            return;
        if(millis() - lastRunMotorTime > MOTOR_AUTO_STOP_MS) {
            log(WARNING, "AutoStop TRIGGERED!\n");
            shortBreak();
        }
    }
    uint8_t ACK_FALSE[1] = {0x00};
    uint8_t ACK_TRUE[1] = {0x01};

    void Ack(bool status) {
        log(PROCESSED, "Motor Ack!\n");
        if (status)
            send(ACK_TRUE, 1);
        else
            send(ACK_FALSE, 1);
    }


    void forward();

    void backward();

    void left();

    void right();

    void shortBreak();

protected:
    bool motorRunning;
    unsigned long motorChangedTime;
    unsigned long lastRunMotorTime;
    String carName;
};


#endif //ESP_CAR_H
