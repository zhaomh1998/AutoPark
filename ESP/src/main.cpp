#include "lib/Communication/master.h"
#include "../lib/SerialCommand/SerialCommand.h"
#include "lib/Util/RGB.h"

// Roles: MASTER, FLOOR1,2,3, CAR1,2,3
const int MY_ROLE = MASTER;
RGB statusLED(15);
master now(macs[MY_ROLE]);

void initialize() {
//    Serial.begin(115200);
    Serial.println("Hi");
    Serial.println(whoIsThis(macs[MY_ROLE]));
    pinMode(12, INPUT);
}


void setup() {
    statusLED.processing();
    initialize();
    statusLED.ready();
}

void loop() {
    if (digitalRead(12)) {
        statusLED.error();
    } else
        statusLED.processing();

    yield();
    if (digitalRead(12)) {
        static uint8_t fwd[] = {0x00};
        now.send(macs[CAR1], fwd, 1);
    }
}

//#include "lib/Util/AutoParkConfig.h"
//#include "lib/Module/AutoParkCar.h"
//
//AutoParkCar car1(CAR1,true);
//
//void setup() {
//
//}
//
//void loop() {
//
//}