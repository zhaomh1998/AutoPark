//#include "lib/Communication/master.h"
//#include "../lib/SerialCommand/SerialCommand.h"
//#include "lib/Util/RGB.h"
//
//// Roles: MASTER, FLOOR1,2,3, CAR1,2,3
//const int MY_ROLE = MASTER;
//RGB statusLED(15);
//master now(macs[MY_ROLE]);
//
//void initialize() {
////    Serial.begin(115200);
//    Serial.println("Hi");
//    Serial.println(whoIsThis(macs[MY_ROLE]));
//    pinMode(12, INPUT);
//}
//
//
//void setup() {
//    statusLED.processing();
//    initialize();
//    statusLED.ready();
//}
//
//void loop() {
//    if (digitalRead(12)) {
//        statusLED.error();
//    } else
//        statusLED.processing();
//
//    yield();
//    if (digitalRead(12)) {
//        static uint8_t fwd[] = {0x00};
//        now.send(macs[CAR1], fwd, 1);
//    }
//}

// CAR ----------------------------------------------------------------------------
#include "lib/Util/AutoParkConfig.h"
#include "lib/Module/AutoParkCar.h"

AutoParkCar *car1 = nullptr;

void setup() {
    delay(1000);
    car1 = new AutoParkCar(CAR1, true);
}

void loop() {
    car1->messageHandler();
    car1->autoStop();
    yield();
}

// Master ----------------------------------------------------------------------------

//#include "lib/Communication/master.h"
//#include "lib/Util/AutoParkConfig.h"
//
//master *ms;
//
//void setup() {
//    ms = new master(MASTER, true);
//}
//
//uint8_t forward[1] = {0x00};
//uint8_t backward[1] = {0x01};
//uint8_t shortBreak[1] = {0x04};
//
//void loop() {
//    ms->send(ms->macs[CAR1], forward, 1);
//    delay(500);
//    ms->send(ms->macs[CAR1], backward, 1);
//    delay(500);
//    ms->send(ms->macs[CAR1], shortBreak, 1);
//    delay(500);
//    yield();
//    Serial.println(ESP.getFreeHeap());
//    Serial.println(ESP.getHeapFragmentation());
//}