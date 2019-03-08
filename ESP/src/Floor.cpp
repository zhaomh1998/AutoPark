//
// Created by Scott Zhao on 2019-03-03.
//

#include "lib/Module/AutoParkFloor.h"

AutoParkFloor *APFloor = nullptr;

int SERIAL_BAUD_RATE = 115200;

void setup() {
    APFloor = new AutoParkFloor(FLOOR3, MASTER, true);
}

void loop() {
    APFloor->messageHandler();
    yield();
}