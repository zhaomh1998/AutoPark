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
    if(!debugAssert(messageLen == 4, ((String)"Received command from master with invalid length, expected 4, received" + (String)messageLen))) return false;
    if(!debugAssert(messageData[0] == 0x01, "Error 206 Unexpected Message Target")) return false; // Target not floor(0x01)
    switch(messageData[1]) { // Message target
        case 0x00:  // Status Update
            if(messageData[2] == 0x00) reportStatus();
            else {debugSend("Error 201 Unexpected Data[2]"); return false;}
            break;
        case 0x01:  // Calibration
            send(Ack(calibrateCart()), 2); break;
        case 0x02:  // Move cart
            switch(messageData[2]) {
                case 0x00: send(Ack(moveCartTo(ELEVATOR_POS)), 2); break;
                case 0x01: send(Ack(moveCartTo(LOT0_POS)), 2); break;
                case 0x02: send(Ack(moveCartTo(LOT1_POS)), 2); break;
                case 0x03: send(Ack(moveCartTo(LOT2_POS)), 2); break;
                default: debugSend("Error 202 Unexpected Data[2]"); return false;
            }
            break;
        case 0x03:  // Move Car
            uint8_t carIndex;
            switch(messageData[2]) {
                case 0x00: carIndex = CAR1; break;
                case 0x01: carIndex = CAR2; break;
                case 0x02: carIndex = CAR3; break;
                default: debugSend("Error 203 Unexpected Data[2]"); return false;
            }
            switch(messageData[3]) {
                case 0x00: carEnterLot(carIndex); break;
                case 0x01: carEnterElevator(carIndex); break;
                case 0x02: carBackUp(carIndex); break;
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
        case 0x00:
            carMessage = true;
            switch(messageData[1]) {
                case 0x01:
                    carAck = true; break;
                case 0x00:
                    carAck = false; break;
                default:
                    debugSend("Error 207 Unexpected Ack Data[1]"); return false;
            }
            break;
        default: debugSend("Error 208 Unexpected Data[0] from car"); return false;
    }
    return true;
}
