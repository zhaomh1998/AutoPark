//
// Created by Scott Zhao on 2019-02-22.
//

#include "lib/Module/AutoParkCar.h"

AutoParkCar *carInstancePtr = nullptr;

void AutoParkCar::forward() {
    info("Forward");
    motorRunning = true;
    lastRunMotorTime = millis();
    digitalWrite(MOTOR_A_CONTROL_1, LOW);
    digitalWrite(MOTOR_A_CONTROL_2, HIGH);
    digitalWrite(MOTOR_B_CONTROL_1, LOW);
    digitalWrite(MOTOR_B_CONTROL_2, HIGH);
}

void AutoParkCar::backward() {
    Serial.println("enter");
    info("Backward");
    Serial.println(1);
    motorRunning = true;
    Serial.println(2);
    lastRunMotorTime = millis();
    Serial.println(3);
    digitalWrite(MOTOR_A_CONTROL_1, HIGH);
    digitalWrite(MOTOR_A_CONTROL_2, LOW);
    digitalWrite(MOTOR_B_CONTROL_1, HIGH);
    digitalWrite(MOTOR_B_CONTROL_2, LOW);
}

void AutoParkCar::left() {
    info("Left");
    motorRunning = true;
    lastRunMotorTime = millis();
    digitalWrite(MOTOR_A_CONTROL_1, LOW);
    digitalWrite(MOTOR_A_CONTROL_2, HIGH);
    digitalWrite(MOTOR_B_CONTROL_1, HIGH);
    digitalWrite(MOTOR_B_CONTROL_2, LOW);
}

void AutoParkCar::right() {
    info("Right");
    motorRunning = true;
    lastRunMotorTime = millis();
    digitalWrite(MOTOR_A_CONTROL_1, HIGH);
    digitalWrite(MOTOR_A_CONTROL_2, LOW);
    digitalWrite(MOTOR_B_CONTROL_1, LOW);
    digitalWrite(MOTOR_B_CONTROL_2, HIGH);
}

void AutoParkCar::shortBreak() {
    info("Break");
    motorRunning = false;
    digitalWrite(MOTOR_A_CONTROL_1, HIGH);
    digitalWrite(MOTOR_A_CONTROL_2, HIGH);
    digitalWrite(MOTOR_B_CONTROL_1, HIGH);
    digitalWrite(MOTOR_B_CONTROL_2, HIGH);
    statusLED.ready();
}

AutoParkCar::AutoParkCar(uint8_t myCarIndex, bool debugMode) : slave(myCarIndex, MASTER, debugMode), statusLED(15) {
    carName = whoIsThis(macs[myCarIndex]);
    debug(carName + " Initialized");
    statusLED.ready();
    carInstancePtr = this;
    setMsgCallback();

    // Set motor PWM
    pinMode(MOTOR_A_CONTROL_1, OUTPUT);
    pinMode(MOTOR_A_CONTROL_2, OUTPUT);
    pinMode(MOTOR_B_CONTROL_1, OUTPUT);
    pinMode(MOTOR_B_CONTROL_2, OUTPUT);
    pinMode(MOTOR_PWN_PIN, OUTPUT);
    shortBreak();
    //    analogWrite(MOTOR_PWN_PIN, CAR_SPEED);
    digitalWrite(MOTOR_PWN_PIN, LOW);
    motorRunning = false;
    motorChangedTime = millis();
    lastRunMotorTime = millis();
}

void AutoParkCar::commandDecoder(const uint8_t *data) {
    Serial.println("In Decoder");
    if (allowMotorChange()) {
        Serial.println("Allow Motor Change");
        statusLED.processing();
        bool successFlag = true;
        Serial.println("Flag Done");
        switch (data[0]) {  // DATA has length not 0?
            case 0x00:
                Serial.println("FWD CASE");
                forward();
                break;
            case 0x01:
                Serial.println("BWD CASE");
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
                warning("Unresolved command received in car");
                successFlag = false;
        }
        Serial.println("Case done");
        Ack(successFlag);
    } else
        Ack(false);
}
