//
// Created by Scott Zhao on 2019-03-03.
//

#include "lib/Module/AutoParkFloor.h"

AutoParkFloor *APFloor = nullptr;

void setup() {
    APFloor = new AutoParkFloor(FLOOR2, MASTER, true);
//    APFloor->carBackUp(CAR1);
//    delay(500);
//    APFloor->carEnterLot(CAR1);
}

void loop() {
    APFloor->messageHandler();
    yield();
}