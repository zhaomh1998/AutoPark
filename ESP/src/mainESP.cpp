////#include <Arduino.h>
//#include "lib/Communication/master.h"
//#include "../lib/SerialCommand/SerialCommand.h"
////#include "lib/Util/stepper.h"
//#include "lib/Util/RGB.h"
//
//// Roles: MASTER, FLOOR1,2,3, CAR1,2,3
//const int MY_ROLE = MASTER;
//SerialCommand myRIO;
//RGB statusLED(15);
//master now(macs[MY_ROLE]);
//
//void singleCommand();
//
//void processCommand();
//
//void LED_on();
//
//void initialize() {
//    Serial.begin(115200);
//    myRIO.addCommand("ON", LED_on);          // Turns LED on
//    myRIO.addCommand("P", processCommand);
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
//void singleCommand() {
//    Serial.println("ON");
//}
//
//void processCommand() {
//    int aNumber;
//    char *arg;
//
//    Serial.println("We're in processCommand");
//    arg = myRIO.next();
//    if (arg != NULL) {
//        aNumber = atoi(arg);    // Converts a char string to an integer
//        Serial.print("First argument was: ");
//        Serial.println(aNumber);
//    } else {
//        Serial.println("No arguments");
//    }
//}
//
//void LED_on() {
//    Serial.println("LED on");
//}