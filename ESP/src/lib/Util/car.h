//
// Created by Scott Zhao on 2019-02-22.
//
#ifndef ESP_CAR_H
#define ESP_CAR_H

#include "../Communication/slave.h"
#include <Arduino.h>

#define MOTOR_A_CONTROL_1 4
#define MOTOR_A_CONTROL_2 5
#define MOTOR_B_CONTROL_1 14
#define MOTOR_B_CONTROL_2 12
#define MAX_MOTOR_REFRESH_MS 200

class car : public slave {
public:
    // TODO: ctor

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
};


#endif //ESP_CAR_H
