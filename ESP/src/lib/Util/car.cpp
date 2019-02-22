//
// Created by Scott Zhao on 2019-02-22.
//

#include "car.h"

void car::forward() {
    if (allowMotorChange()) {
        Serial.println("Forward");
        digitalWrite(MOTOR_A_CONTROL_1, LOW);
        digitalWrite(MOTOR_A_CONTROL_2, HIGH);
        digitalWrite(MOTOR_B_CONTROL_1, LOW);
        digitalWrite(MOTOR_B_CONTROL_2, HIGH);
    }
}

void car::backward() {
    if (allowMotorChange()) {
        Serial.println("Backward");
        digitalWrite(MOTOR_A_CONTROL_1, HIGH);
        digitalWrite(MOTOR_A_CONTROL_2, LOW);
        digitalWrite(MOTOR_B_CONTROL_1, HIGH);
        digitalWrite(MOTOR_B_CONTROL_2, LOW);
    }
}

void car::left() {
    if (allowMotorChange()) {
        Serial.println("Left");
        digitalWrite(MOTOR_A_CONTROL_1, LOW);
        digitalWrite(MOTOR_A_CONTROL_2, HIGH);
        digitalWrite(MOTOR_B_CONTROL_1, HIGH);
        digitalWrite(MOTOR_B_CONTROL_2, LOW);
    }
}

void car::right() {
    if (allowMotorChange()) {
        Serial.println("Right");
        digitalWrite(MOTOR_A_CONTROL_1, HIGH);
        digitalWrite(MOTOR_A_CONTROL_2, LOW);
        digitalWrite(MOTOR_B_CONTROL_1, LOW);
        digitalWrite(MOTOR_B_CONTROL_2, HIGH);
    }
}

void car::shortBreak() {
    if (allowMotorChange()) {
        Serial.println("Break");
        digitalWrite(MOTOR_A_CONTROL_1, HIGH);
        digitalWrite(MOTOR_A_CONTROL_2, HIGH);
        digitalWrite(MOTOR_B_CONTROL_1, HIGH);
        digitalWrite(MOTOR_B_CONTROL_2, HIGH);
    }
}
