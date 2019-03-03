////#include "lib/Communication/master.h"
//#include "../lib/SerialCommand/SerialCommand.h"
//#include "lib/Util/RGB.h"
//
//SerialCommand myRIO;
//RGB statusLED(15);
//
//void parkCar();
//void takeCar();
//
//void initialize() {
//    Serial.begin(115200);
//    myRIO.addCommand("IN", parkCar);
//    myRIO.addCommand("OUT", takeCar);
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
//    myRIO.readSerial();
//    yield();
////    now.send(macs[CAR1], macs[MY_ROLE], 6);
////    delay(500);
//}
//
//void parkCar() {
//    int car;
//    int floor;
//    int lot;
//
//    char *arg;
//
//    car = atoi(myRIO.next());
//    floor = atoi(myRIO.next());
//    lot = atoi(myRIO.next());
//    statusLED.warning();
//    delay(2000);
//    statusLED.ready();
//    Serial.println("DONE");
//}
//
//void takeCar() {
//    int car;
//    int floor;
//    int lot;
//
//    char *arg;
//
//    car = atoi(myRIO.next());
//    floor = atoi(myRIO.next());
//    lot = atoi(myRIO.next());
//    statusLED.error();
//    delay(2000);
//    statusLED.ready();
//    Serial.println("DONE");
//}
