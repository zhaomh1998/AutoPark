////
//// Created by Scott Zhao on 2019-02-22.
////
//
//#include "AutoParkCar.h"
//
//AutoParkCar *carInstancePtr = nullptr;
//
//void AutoParkCar::forward() {
//    if (allowMotorChange()) {
//        Serial.println("Forward");
//        digitalWrite(MOTOR_A_CONTROL_1, LOW);
//        digitalWrite(MOTOR_A_CONTROL_2, HIGH);
//        digitalWrite(MOTOR_B_CONTROL_1, LOW);
//        digitalWrite(MOTOR_B_CONTROL_2, HIGH);
//    }
//}
//
//void AutoParkCar::backward() {
//    if (allowMotorChange()) {
//        Serial.println("Backward");
//        digitalWrite(MOTOR_A_CONTROL_1, HIGH);
//        digitalWrite(MOTOR_A_CONTROL_2, LOW);
//        digitalWrite(MOTOR_B_CONTROL_1, HIGH);
//        digitalWrite(MOTOR_B_CONTROL_2, LOW);
//    }
//}
//
//void AutoParkCar::left() {
//    if (allowMotorChange()) {
//        Serial.println("Left");
//        digitalWrite(MOTOR_A_CONTROL_1, LOW);
//        digitalWrite(MOTOR_A_CONTROL_2, HIGH);
//        digitalWrite(MOTOR_B_CONTROL_1, HIGH);
//        digitalWrite(MOTOR_B_CONTROL_2, LOW);
//    }
//}
//
//void AutoParkCar::right() {
//    if (allowMotorChange()) {
//        Serial.println("Right");
//        digitalWrite(MOTOR_A_CONTROL_1, HIGH);
//        digitalWrite(MOTOR_A_CONTROL_2, LOW);
//        digitalWrite(MOTOR_B_CONTROL_1, LOW);
//        digitalWrite(MOTOR_B_CONTROL_2, HIGH);
//    }
//}
//
//void AutoParkCar::shortBreak() {
//    if (allowMotorChange()) {
//        Serial.println("Break");
//        digitalWrite(MOTOR_A_CONTROL_1, HIGH);
//        digitalWrite(MOTOR_A_CONTROL_2, HIGH);
//        digitalWrite(MOTOR_B_CONTROL_1, HIGH);
//        digitalWrite(MOTOR_B_CONTROL_2, HIGH);
//    }
//}
