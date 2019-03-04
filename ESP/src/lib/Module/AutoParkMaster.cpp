////
//// Created by Scott Zhao on 2019-03-03.
////
//
//#include "AutoParkMaster.h"
//
//void AutoParkMaster::parkCar() {
//    delay(50);  // Give time for all params to be sent
//    Serial.println("ParkedCar");
//    int car;
//    int floor;
//    int lot;
//
//    char *carArg = myRIO.next();
//    char *floorArg = myRIO.next();
//    char *lotArg = myRIO.next();
//
//    if(carArg != NULL && floorArg != NULL && lotArg != NULL) {
//        car = atoi(carArg);
//        floor = atoi(carArg);
//        lot = atoi(carArg);
//        Serial.println("DONE");
//        statusLED->ready();
//        Serial.println("EXIT");
//    }
//    else {
//
//    }
//}
//
//void AutoParkMaster::takeCar() {
//    delay(50);  // Give time for all params to be sent
//    Serial.println("TakeCar");
//    int car;
//    int floor;
//    int lot;
//
//    char *carArg = myRIO.next();
//    char *floorArg = myRIO.next();
//    char *lotArg = myRIO.next();
//
//    if(carArg != NULL && floorArg != NULL && lotArg != NULL) {
////        statusLED.warning();
//        car = atoi(carArg);
//        floor = atoi(carArg);
//        lot = atoi(carArg);
//        delay(2000);
//        Serial.println("DONE");
////        statusLED.ready();
//    }
//    else {
//        Serial.println("ERROR Insufficient Arguments");
////        statusLED.error();
//    }
//}