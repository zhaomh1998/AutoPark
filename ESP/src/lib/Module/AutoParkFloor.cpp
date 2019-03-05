//
// Created by Scott Zhao on 2019-03-03.
//

#include "AutoParkFloor.h"

AutoParkFloor::AutoParkFloor(uint8_t floorIndex, uint8_t masterIndex, bool debugMode) : slave(floorIndex, masterIndex, debugMode){
    carMessage = false;
    carAck = false;
    addPeer(CAR1);
    addPeer(CAR2);
    addPeer(CAR3);
    pinMode(CART_PHOTORESISTOR_1, INPUT);
    pinMode(LOT_PHOTORESISTOR_2, INPUT);
    pinMode(CART_LS_R, INPUT);
    pinMode(CART_LS_L, INPUT);
//    if(!calibrateSensor()) {
//        statusLED.error();
//        debugSendLn("Error");
//    }
}

bool AutoParkFloor::messageHandler() {
    if(!messagePending)
        return false;
    else {
        int messageSender = whoIsThisIndex(messageOrigin);
        switch(messageSender) {
            case MASTER: masterMessageHandler(); break;
            case CAR1: carMessageHandler(CAR1); break;
            case CAR2: carMessageHandler(CAR2); break;
            case CAR3: carMessageHandler(CAR3); break;
            default: debugSend("Error 200 Unexpected Sender: " + (String) messageSender);
        }
        messagePending = false;
        return true;
    }
}

bool AutoParkFloor::masterMessageHandler() {
    if(!debugAssert(messageLen == 4,
            ((String)"Received command from master with invalid length, expected 4, received" + (String)messageLen))) return false;
    if(!debugAssert(messageData[0] == TARGET_FLOOR, "Error 206 Unexpected Message Target")) return false;  // Target not floor
    switch(messageData[1]) { // Message target
        case FLOOR_STATUS_UPDATE_CMD:  // Status Update
            if(messageData[2] == FLOOR_STATUS_REQUEST) reportStatus();
            else {debugSend("Error 201 Unexpected Data[2]"); return false;}
            break;
        case FLOOR_CALIBRATE_CMD:  // Calibration
            send(Ack(calibrateCart()), 2); break;
        case FLOOR_MOVE_CART_CMD:  // Move cart
            switch(messageData[2]) {
                case FLOOR_ELEVATOR: send(Ack(moveCartTo(ELEVATOR_POS)), 2); break;
                case FLOOR_LOT1: send(Ack(moveCartTo(LOT1_POS)), 2); break;
                case FLOOR_LOT2: send(Ack(moveCartTo(LOT2_POS)), 2); break;
                case FLOOR_LOT3: send(Ack(moveCartTo(LOT3_POS)), 2); break;
                default: debugSend("Error 202 Unexpected Data[2]"); return false;
            }
            break;
        case FLOOR_MOVE_CAR_CMD:  // Move Car
            uint8_t carIndex;
            switch(messageData[2]) {
                case FLOOR_CAR1: carIndex = CAR1; break;
                case FLOOR_CAR2: carIndex = CAR2; break;
                case FLOOR_CAR3: carIndex = CAR3; break;
                default: debugSend("Error 203 Unexpected Data[2]"); return false;
            }
            switch(messageData[3]) {
                case FLOOR_CAR_IN_LOT: carEnterLot(carIndex); break;
                case FLOOR_CAR_IN_ELEV: carEnterElevator(carIndex); break;
                case FLOOR_CAR_TO_CART: carBackUp(carIndex); break;
                default: debugSend("Error 204 Unexpected Data[3]"); return false;
            }
            break;
        default: debugSend("Error 205 Unexpected Data[1]"); return false;
    }
    return true;
}

bool AutoParkFloor::carMessageHandler(int carIndex) {
    debugAssert(messageLen == 4, ((String)"Received command from master with invalid length, expected 4, received" + (String)messageLen));
    switch(messageData[0]) { // Message target
        case TARGET_ACK:
            carMessage = true;
            switch(messageData[1]) {
                case ACK_TRUE:
                    carAck = true; break;
                case ACK_FALSE:
                    carAck = false; break;
                default:
                    debugSend("Error 207 Unexpected Ack Data[1]"); return false;
            }
            break;
        default: debugSend("Error 208 Unexpected Data[0] from car"); return false;
    }
    return true;
}
