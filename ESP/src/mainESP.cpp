//#include "../lib/SerialCommand/SerialCommand.h"
//#include "lib/Util/RGB.h"
//#include "lib/Communication/master.h"
//
//SerialCommand myRIO;
//master *ESPNetwork = nullptr;
//
//bool commandStatus;
//
//
//
//void initialize() {
//    Serial.begin(115200);
//    myRIO.addCommand("IN", parkCar);
//    myRIO.addCommand("OUT", takeCar);
//}
//
//
//void setup() {
//    statusLED = new RGB(15);
//    initialize();
//}
//
//void loop() {
//    myRIO.readSerial();
//    yield();
////    now.send(macs[CAR1], macs[MY_ROLE], 6);
////    delay(500);
//}