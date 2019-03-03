//
// Created by Scott Zhao on 2019-02-22.
//

#include "lib/Module/AutoParkCar.h"

AutoParkCar *carInstancePtr = nullptr;

void AutoParkCar::forward() {
    log(PROCESSED, "Forward");
    motorRunning = true;
    lastRunMotorTime = millis();
    digitalWrite(MOTOR_A_CONTROL_1, LOW);
    digitalWrite(MOTOR_A_CONTROL_2, HIGH);
    digitalWrite(MOTOR_B_CONTROL_1, LOW);
    digitalWrite(MOTOR_B_CONTROL_2, HIGH);
}

void AutoParkCar::backward() {
    log(PROCESSED, "Backward\n");
    motorRunning = true;
    lastRunMotorTime = millis();
    digitalWrite(MOTOR_A_CONTROL_1, HIGH);
    digitalWrite(MOTOR_A_CONTROL_2, LOW);
    digitalWrite(MOTOR_B_CONTROL_1, HIGH);
    digitalWrite(MOTOR_B_CONTROL_2, LOW);
}

void AutoParkCar::left() {
    log(PROCESSED, "Left\n");
    motorRunning = true;
    lastRunMotorTime = millis();
    digitalWrite(MOTOR_A_CONTROL_1, LOW);
    digitalWrite(MOTOR_A_CONTROL_2, HIGH);
    digitalWrite(MOTOR_B_CONTROL_1, HIGH);
    digitalWrite(MOTOR_B_CONTROL_2, LOW);
}

void AutoParkCar::right() {
    log(PROCESSED, "Right\n");
    motorRunning = true;
    lastRunMotorTime = millis();
    digitalWrite(MOTOR_A_CONTROL_1, HIGH);
    digitalWrite(MOTOR_A_CONTROL_2, LOW);
    digitalWrite(MOTOR_B_CONTROL_1, LOW);
    digitalWrite(MOTOR_B_CONTROL_2, HIGH);
}

void AutoParkCar::shortBreak() {
    log(PROCESSED, "Break\n");
    motorRunning = false;
    digitalWrite(MOTOR_A_CONTROL_1, HIGH);
    digitalWrite(MOTOR_A_CONTROL_2, HIGH);
    digitalWrite(MOTOR_B_CONTROL_1, HIGH);
    digitalWrite(MOTOR_B_CONTROL_2, HIGH);
    statusLED.ready();
}

AutoParkCar::AutoParkCar(uint8_t myCarIndex, bool debugMode) : slave(myCarIndex, MASTER, debugMode) {
    carName = whoIsThis(macs[myCarIndex]);
    log(PROCESSED, carName + " Initialized\n");
    statusLED.ready();
    carInstancePtr = this;

    // Set motor PWM
    pinMode(MOTOR_A_CONTROL_1, OUTPUT);
    pinMode(MOTOR_A_CONTROL_2, OUTPUT);
    pinMode(MOTOR_B_CONTROL_1, OUTPUT);
    pinMode(MOTOR_B_CONTROL_2, OUTPUT);
    pinMode(MOTOR_PWN_PIN, OUTPUT);
    shortBreak();
//    analogWrite(MOTOR_PWN_PIN, CAR_SPEED);
    digitalWrite(MOTOR_PWN_PIN, HIGH);
    motorRunning = false;
    motorChangedTime = millis();
    lastRunMotorTime = millis();
}

void AutoParkCar::commandDecoder(const uint8_t *data) {
    if (allowMotorChange()) {
        statusLED.processing();
        bool successFlag = true;
        switch (data[0]) {  // DATA has length not 0?
            case 0x00:
                forward();
                break;
            case 0x01:
                backward();
                break;
            case 0x02:
                left();
                break;
            case 0x03:
                right();
                break;
            case 0x04:
                shortBreak();
                break;
            default:
                log(WARNING, "Unresolved command received in car");
                successFlag = false;
        }
        Ack(successFlag);

    } else
        Ack(false);
}
