//
// Created by Scott Zhao on 2019-03-03.
//

#include "AutoParkFloor.h"

AutoParkFloor::AutoParkFloor(uint8_t floorIndex, uint8_t masterIndex, bool debugMode) : slave(floorIndex, masterIndex, debugMode){
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
