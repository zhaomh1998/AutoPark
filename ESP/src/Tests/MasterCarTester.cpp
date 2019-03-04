//
// Created by Scott Zhao on 2019-03-03.
//


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