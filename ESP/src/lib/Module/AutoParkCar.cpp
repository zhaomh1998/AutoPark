//
// Created by Scott Zhao on 2019-02-22.
//

#include "lib/Module/AutoParkCar.h"

AutoParkCar::AutoParkCar(uint8_t carIndex, bool debugMode) : slave(carIndex, MASTER, debugMode) {
    carName = whoIsThis(macs[carIndex]);
    log(PROCESSED, carName + " Initialized\n");
    statusLED.ready();

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

bool AutoParkCar::messageHandler() {  // override this method in subclasses
    if(!messagePending)
        return false;
    else {
        printESPNowMsg(RECEIVE, messageOrigin, messageData, messageLen);
        if(messageLen == 1) {
            commandDecoder(messageData);
            masterMacAddr = messageOrigin;  // Switch master in case a new master appears
        }
        else{
            log(WARNING, "^^^^^Unexpected ESPNow Message!\n");
        }
        messagePending = false;
        return true;
    }

}

void AutoParkCar::commandDecoder(const uint8_t *data) {
    if(data[0] == 0x04) { // Stop
        shortBreak();
        Ack(true);
    }
    else if (allowMotorChange()) {
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
            default:
                log(WARNING, "Unresolved command received in car\n");
                successFlag = false;
        }
        Ack(successFlag);

    } else
        Ack(false);
}

void AutoParkCar::Ack(bool status) {
    log(PROCESSED, "Motor Ack!\n");
    if (status)
        send(ACK_TRUE, 1);
    else
        send(ACK_FALSE, 1);
}


bool AutoParkCar::allowMotorChange() {
    unsigned long timeNow = millis();
    if (timeNow - motorChangedTime > MAX_MOTOR_REFRESH_MS) {
        motorChangedTime = timeNow;
        return true;
    }
    return false;
}

void AutoParkCar::autoStop() {
    if(!motorRunning)
        return;
    if(millis() - lastRunMotorTime > MOTOR_AUTO_STOP_MS) {
        log(WARNING, "AutoStop TRIGGERED!\n");
        shortBreak();
    }
}


void AutoParkCar::forward() {
    log(PROCESSED, "Forward\n");
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
