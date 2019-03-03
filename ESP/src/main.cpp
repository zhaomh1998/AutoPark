// CAR ----------------------------------------------------------------------------
//#include "lib/Util/AutoParkConfig.h"
//#include "lib/Module/AutoParkCar.h"
//
//AutoParkCar *car1 = nullptr;
//
//void setup() {
//    delay(200);
//    car1 = new AutoParkCar(CAR1, true);
//}
//
//void loop() {
//    car1->messageHandler();
//    car1->autoStop();
//    yield();
//}

// Master Test Car ----------------------------------------------------------------------------

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

// MainESP ----------------------------------------------------------------------
#include "../lib/SerialCommand/SerialCommand.h"
#include "lib/Util/RGB.h"

SerialCommand myRIO;
RGB statusLED(15);

void parkCar();
void takeCar();

void initialize() {
    Serial.begin(115200);
    myRIO.addCommand("IN", parkCar);
    myRIO.addCommand("OUT", takeCar);
}


void setup() {
    statusLED.processing();
    initialize();
    statusLED.ready();
}

void loop() {
    myRIO.readSerial();
    delay(100);
    yield();
//    now.send(macs[CAR1], macs[MY_ROLE], 6);
//    delay(500);
}

void parkCar() {
    Serial.println("ParkedCar");
    int car;
    int floor;
    int lot;

    char *arg;

    car = atoi(myRIO.next());
    floor = atoi(myRIO.next());
    lot = atoi(myRIO.next());
    statusLED.warning();
    delay(2000);
    statusLED.ready();
    Serial.println("DONE");
}

void takeCar() {
    int car;
    int floor;
    int lot;

    char *arg;

    car = atoi(myRIO.next());
    floor = atoi(myRIO.next());
    lot = atoi(myRIO.next());
    statusLED.error();
    delay(2000);
    statusLED.ready();
    Serial.println("DONE");
}
