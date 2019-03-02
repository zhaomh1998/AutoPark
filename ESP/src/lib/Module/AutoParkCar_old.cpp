////
//// Created by Scott Zhao on 2019-02-22.
////
//
//#include "lib/Module/AutoParkCar.h"
//
//AutoParkCar *carInstancePtr = nullptr;
//
//void AutoParkCar::forward() {
//    info("Forward");
//    motorRunning = true;
//    lastRunMotorTime = millis();
//    digitalWrite(MOTOR_A_CONTROL_1, LOW);
//    digitalWrite(MOTOR_A_CONTROL_2, HIGH);
//    digitalWrite(MOTOR_B_CONTROL_1, LOW);
//    digitalWrite(MOTOR_B_CONTROL_2, HIGH);
//}
//
//void AutoParkCar::backward() {
//    info("Backward");
//    motorRunning = true;
//    lastRunMotorTime = millis();
//    digitalWrite(MOTOR_A_CONTROL_1, HIGH);
//    digitalWrite(MOTOR_A_CONTROL_2, LOW);
//    digitalWrite(MOTOR_B_CONTROL_1, HIGH);
//    digitalWrite(MOTOR_B_CONTROL_2, LOW);
//}
//
//void AutoParkCar::left() {
//    info("Left");
//    motorRunning = true;
//    lastRunMotorTime = millis();
//    digitalWrite(MOTOR_A_CONTROL_1, LOW);
//    digitalWrite(MOTOR_A_CONTROL_2, HIGH);
//    digitalWrite(MOTOR_B_CONTROL_1, HIGH);
//    digitalWrite(MOTOR_B_CONTROL_2, LOW);
//}
//
//void AutoParkCar::right() {
//    info("Right");
//    motorRunning = true;
//    lastRunMotorTime = millis();
//    digitalWrite(MOTOR_A_CONTROL_1, HIGH);
//    digitalWrite(MOTOR_A_CONTROL_2, LOW);
//    digitalWrite(MOTOR_B_CONTROL_1, LOW);
//    digitalWrite(MOTOR_B_CONTROL_2, HIGH);
//}
//
//void AutoParkCar::shortBreak() {
//    info("Break");
//    motorRunning = false;
//    digitalWrite(MOTOR_A_CONTROL_1, HIGH);
//    digitalWrite(MOTOR_A_CONTROL_2, HIGH);
//    digitalWrite(MOTOR_B_CONTROL_1, HIGH);
//    digitalWrite(MOTOR_B_CONTROL_2, HIGH);
//    statusLED.ready();
//}
//
//AutoParkCar::AutoParkCar(uint8_t myCarIndex, bool debugMode) : slave(myCarIndex, MASTER, debugMode), statusLED(15) {
//    carName = whoIsThis(macs[myCarIndex]);
//    debug(carName + " Initialized");
//    statusLED.ready();
//    carInstancePtr = this;
//    setMsgCallback();
//
//    // Set motor PWM
//    analogWrite(MOTOR_PWN_PIN, CAR_SPEED);
//    shortBreak();
//    motorRunning = false;
//    motorChangedTime = millis();
//    lastRunMotorTime = millis();
//}
//
//void AutoParkCar::commandDecoder(const uint8_t *data) {
//    if (allowMotorChange()) {
//        statusLED.processing();
//        bool successFlag = true;
//        switch (data[0]) {
//            case 0x00:
//                forward();
//                break;
//            case 0x01:
//                backward();
//                break;
//            case 0x02:
//                left();
//                break;
//            case 0x03:
//                right();
//                break;
//            case 0x04:
//                shortBreak();
//                break;
//            default:
//                logHandle.warning("Unresolved command received in car");
//                successFlag = false;
//        }
//        Ack(successFlag);
//    } else
//        Ack(false);
//}
