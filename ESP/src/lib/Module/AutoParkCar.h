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
#define MAX_MOTOR_REFRESH_MS 200

class AutoParkCar;

extern AutoParkCar *carInstancePtr;

class AutoParkCar : public slave {
public:
    AutoParkCar(int whichCar, bool debug) : slave(macs[whichCar], macs[MASTER], debug), statusLED(15) {
        carName = whoIsThis(macs[whichCar]);
        Serial.println("Size of macs in class:");
        Serial.println(macs.size());
        logger::serialLogByteStatic(macs[whichCar][5]);
        logHandle.debug(carName + " Initialized");
        statusLED.ready();
        carInstancePtr = this;
    }

    void setMsgCallback() override {
        esp_now_register_recv_cb(carMessageHandler);
    }

    static void carMessageHandler(uint8_t *mac, uint8_t *data, uint8_t len) {
        auto carInstance = carInstancePtr;
        if (carInstance->isDebugMode)
            logger::printESPNowMsg(RECEIVE, mac, data, len);
        carInstance->commandDecoder(data);
    }

    void commandDecoder(const uint8_t *data) {
        statusLED.processing();
        logHandle.debug("Decoding data");
        switch (data[0]) {
            case 0x00:
                logHandle.info("00");
                break;
            case 0x02:
                logHandle.info("01");
                break;
            default:
                logHandle.warning("Unresolved command received in car");
        }

    }

    bool allowMotorChange() {
        unsigned long timeNow = millis();
        if (timeNow - motorChangedTime > MAX_MOTOR_REFRESH_MS) {
            motorChangedTime = timeNow;
            return true;
        }
        return false;
    }

    void forward();

    void backward();

    void left();

    void right();

    void shortBreak();

protected:
    unsigned long motorChangedTime = 0;
    String carName;
    RGB statusLED;
};


#endif //ESP_CAR_H
