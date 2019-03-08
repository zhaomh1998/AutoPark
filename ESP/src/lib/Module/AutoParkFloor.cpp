//
// Created by Scott Zhao on 2019-03-03.
//

#include "AutoParkFloor.h"
int16_t AutoParkFloor::stepperPosition;
int16_t AutoParkFloor::targetStepCount;
int16_t AutoParkFloor::currentStepCount;
int AutoParkFloor::stepperDirection;
Ticker AutoParkFloor::stepperCallbacker;
bool AutoParkFloor::calibFinished;

AutoParkFloor::AutoParkFloor(uint8_t floorIndex, uint8_t masterIndex, bool debugMode) : slave(floorIndex, masterIndex, debugMode){
    carMessage = false;
    carAck = false;
    addPeer(CAR1);
    addPeer(CAR2);
    addPeer(CAR3);
    thisFloor = floorIndex;

    pinMode(CART_PHOTORESISTOR_1, INPUT);
    pinMode(LOT_PHOTORESISTOR_2, INPUT);
    pinMode(CART_LS_R, INPUT);
    pinMode(CART_LS_L, INPUT);
    pinMode(STEPPER_DIR_PIN, OUTPUT);
    pinMode(STEPPER_STEP_PIN, OUTPUT);
    // Calibrate sensor
    if(!calibrateSensor()) {
        statusLED.error();
        debugSendLn("Error");
    }
    delay(500);
    calibrateCart();
//    delay(500);
//    delay(1000);
//    moveCartTo(LOT1_POS);
//    delay(1000);
////    carEnterLot(CAR1);
//    moveCartTo(LOT2_POS);
//    carEnterLot(CAR1);
////    delay(1000);
//    delay(1000);
//    moveCartTo(LOT3_POS);
//    delay(1000);
//    moveCartTo(LOT2_POS);
//    carBackUp(CAR1);
//    delay(1000);
//    carBackUp(CAR1);
}

bool AutoParkFloor::messageHandler() {
    if(!messagePending)
        return false;
    else {
        int messageSender = whoIsThisIndex(messageOrigin);
//        Serial.println("Sender" + (String) messageSender + "isMaster?" + (String) (messageSender == MASTER));
        printESPNowMsg(RECEIVE, messageOrigin, messageData, messageLen);
        switch(messageSender) {
            case MASTER: masterMessageHandler(); break;
            case CAR1: carMessageHandler(CAR1); break;
            case CAR2: carMessageHandler(CAR2); break;
            case CAR3: carMessageHandler(CAR3); break;
            default: debugSendLn("Error 200 Unexpected Sender: " + (String) messageSender);
        }
        messagePending = false;
        return true;
    }
}

bool AutoParkFloor::masterMessageHandler() {
    if(!debugAssert(messageLen == MSG_LEN,
            ((String)"Received command from master with invalid length, expected 4, received" + (String)messageLen))) return false;
    if(!debugAssert((messageData[0] == TARGET_FLOOR || messageData[0] == TARGET_ACK), "Error 206 Unexpected Message Target")) return false;  // Target not floor
    switch(messageData[0]) {
        case TARGET_FLOOR:
            switch(messageData[1]) { // Message op
                case FLOOR_STATUS_UPDATE_CMD:  // Status Update
                    if(messageData[2] == FLOOR_STATUS_REQUEST) reportStatus();
                    else {debugSendLn("Error 201 Unexpected Data[2]"); return false;}
                    break;
                case FLOOR_CALIBRATE_CMD:  // Calibration
                Serial.println("FLOOR CALIB CMD!!!!!");
                    send(Ack(calibrateCart()), 2); break;
                case FLOOR_MOVE_CART_CMD:  // Move cart
                    switch(messageData[2]) {
                        case FLOOR_ELEVATOR: send(Ack(moveCartTo(ELEVATOR_POS)), MSG_LEN); break;
                        case FLOOR_LOT1: send(Ack(moveCartTo(LOT1_POS)), MSG_LEN); break;
                        case FLOOR_LOT2: send(Ack(moveCartTo(LOT2_POS)), MSG_LEN); break;
                        case FLOOR_LOT3: send(Ack(moveCartTo(LOT3_POS)), MSG_LEN); break;
                        default: debugSendLn("Error 202 Unexpected Data[2]"); return false;
                    }
                    break;
                case FLOOR_MOVE_CAR_CMD:  // Move Car
                    uint8_t carIndex;
                    switch(messageData[2]) {
                        case FLOOR_CAR1: carIndex = CAR1; break;
                        case FLOOR_CAR2: carIndex = CAR2; break;
                        case FLOOR_CAR3: carIndex = CAR3; break;
                        default: debugSendLn("Error 203 Unexpected Data[2]"); return false;
                    }
                    switch(messageData[3]) {
                        case FLOOR_CAR_IN_LOT: send(Ack(carEnterLot (carIndex)), MSG_LEN); break;
                        case FLOOR_CAR_IN_ELEV: send(Ack(carEnterElevator(carIndex)), MSG_LEN); Serial.println("ELE_DONE"); break;
                        case FLOOR_CAR_TO_CART: send(Ack(carBackUp(carIndex)), MSG_LEN); break;
                        default: debugSendLn("Error 204 Unexpected Data[3]"); return false;
                    }
                    break;
                default: debugSendLn("Error 205 Unexpected Data[1]"); return false;
            }
            break;
        case TARGET_ACK: return true;
        default: debugSendLn("Error 215 Unexpected Master message target[0]:" + getByte(messageData[0]));
    }
    return true;
}
bool AutoParkFloor::carMessageHandler(int carIndex) {
    debugAssert(messageLen == MSG_LEN, ((String)"Received command from car with invalid length, expected 4, received" + (String)messageLen));
    switch(messageData[0]) { // Message target
        case TARGET_ACK:
            carMessage = true;
            switch(messageData[1]) {
                case ACK_TRUE:
                    carAck = true; break;
                case ACK_FALSE:
                    carAck = false; break;
                default:
                    debugSendLn("Error 207 Unexpected Ack Data[1]"); return false;
            }
            break;
        default: debugSendLn("Error 208 Unexpected Data[0] from car"); return false;
    }
    return true;
}
bool AutoParkFloor::calibrateSensor() {
//    std::vector <bool> calibrated = {false, false, false, false};
//    while(!std::all_of(calibrated.begin(), calibrated.end(), [](bool v) { return v; })) {  // while not all calibrated
//        yield();
//        if(getCartLeftLS() && !calibrated[0]) {
//            statusLED.processing();
//            calibrated[0] = true;
//            debugSendLn("CartLeftLS Calibrated");
//        }
//        if(getCartRightLS() && !calibrated[1]) {
//            statusLED.processing();
//            calibrated[1] = true;
//            debugSendLn("CartRightLS Calibrated");
//
//        }
//        if(getCartLaser() && !calibrated[2]) {
//            statusLED.processing();
//            calibrated[2] = true;
//            debugSendLn("CartLaser Calibrated");
//
//        }
//        if(getLotLaser() && !calibrated[3]) {
//            statusLED.processing();
//            calibrated[3] = true;
//            debugSendLn("LotLaser Calibrated");
//        }
//    }
    debugSendLn("Sensors All calibrated");
    return true;
}
