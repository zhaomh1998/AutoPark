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

class AutoParkCar : public slave {
public:
    AutoParkCar(uint8_t carIndex, bool debugMode);

    // Communication
    bool messageHandler() override;
    void commandDecoder(const uint8_t *data);
//    uint8_t ACK_FALSE[1] = {0x00};
//    uint8_t ACK_TRUE[1] = {0x01};
    void sendAck(bool status);

    // Controllers
    bool allowMotorChange();
    void autoStop(); // If stop command get lost, trigger stop after a timeout

    // Motor Driver Util
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
