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
        masterMacAddr = messageOrigin;  // Switch master in case a new master appears
        commandDecoder(messageData);
        messagePending = false;
        return true;
    }

}

void AutoParkCar::commandDecoder(const uint8_t *data) {
    if(!debugAssert(data[0] == TARGET_CAR, "Error 301 Unexpected data[0]: " + getByte(data[0]))) return;
    if(data[1] == CAR_STOP) { // Stop
        shortBreak();
        sendAck(true);
    }
    else if (allowMotorChange()) {
        statusLED.processing();
        bool successFlag = true;
        switch (data[1]) {  // DATA has length not 0?
            case CAR_FORWARD:
                forward();
                break;
            case CAR_BACKWARD:
                backward();
                break;
            default:
                debugSendLn("Error 302 Unresolved command data[1] received in car" + getByte(data[1]));
                successFlag = false;
        }
        sendAck(successFlag);

    } else
        sendAck(false);
}

void AutoParkCar::sendAck(bool status) {
    log(PROCESSED, "Motor Ack!\n");
    send(Ack(status), MSG_LEN);
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


void AutoParkCar::backward() {
    log(PROCESSED, "Backward\n");
    debugSendLn("Backward");
    motorRunning = true;
    lastRunMotorTime = millis();
    digitalWrite(MOTOR_A_CONTROL_1, LOW);
    digitalWrite(MOTOR_A_CONTROL_2, HIGH);
    digitalWrite(MOTOR_B_CONTROL_1, LOW);
    digitalWrite(MOTOR_B_CONTROL_2, HIGH);
}

void AutoParkCar::forward() {
    log(PROCESSED, "Forward\n");
    debugSendLn("Forward");
    motorRunning = true;
    lastRunMotorTime = millis();
    digitalWrite(MOTOR_A_CONTROL_1, HIGH);
    digitalWrite(MOTOR_A_CONTROL_2, LOW);
    digitalWrite(MOTOR_B_CONTROL_1, HIGH);
    digitalWrite(MOTOR_B_CONTROL_2, LOW);
}

//void AutoParkCar::left() {
//    log(PROCESSED, "Left\n");
//    motorRunning = true;
//    lastRunMotorTime = millis();
//    digitalWrite(MOTOR_A_CONTROL_1, LOW);
//    digitalWrite(MOTOR_A_CONTROL_2, HIGH);
//    digitalWrite(MOTOR_B_CONTROL_1, HIGH);
//    digitalWrite(MOTOR_B_CONTROL_2, LOW);
//}
//
//void AutoParkCar::right() {
//    log(PROCESSED, "Right\n");
//    motorRunning = true;
//    lastRunMotorTime = millis();
//    digitalWrite(MOTOR_A_CONTROL_1, HIGH);
//    digitalWrite(MOTOR_A_CONTROL_2, LOW);
//    digitalWrite(MOTOR_B_CONTROL_1, LOW);
//    digitalWrite(MOTOR_B_CONTROL_2, HIGH);
//}

void AutoParkCar::shortBreak() {
    log(PROCESSED, "Break\n");
    debugSendLn("Break");
    motorRunning = false;
    digitalWrite(MOTOR_A_CONTROL_1, HIGH);
    digitalWrite(MOTOR_A_CONTROL_2, HIGH);
    digitalWrite(MOTOR_B_CONTROL_1, HIGH);
    digitalWrite(MOTOR_B_CONTROL_2, HIGH);
    statusLED.ready();
}
