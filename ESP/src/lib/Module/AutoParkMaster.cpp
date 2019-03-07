//
// Created by Scott Zhao on 2019-03-03.
//

#include "AutoParkMaster.h"
AutoParkMaster* masterInstancePtr = nullptr;

// Serial command handlers
bool AutoParkMaster::RIOCommandPending;
uint8_t AutoParkMaster::RIOCommand;
char *AutoParkMaster::RIOArg1;
char *AutoParkMaster::RIOArg2;
char *AutoParkMaster::RIOArg3;



AutoParkMaster::AutoParkMaster(uint8_t myMacIndex, bool debug) : master(myMacIndex, debug), myRIO() {
    statusLED.processing();
    // Add floors
    addFloor(FLOOR1);

    // Initialize myRIO
    myRIO.addCommand("CAR", carCommandHandler);
    myRIO.addCommand("CART", cartCommandHandler);
    myRIO.setDefaultHandler(unknownCommandHandler);
    masterInstancePtr = this;
}


// UART Command Handler
void AutoParkMaster::carCommandHandler() {
    if(RIOCommandPending) {  // There is an unhandled command
        String errorMsg = "ERROR 10 There is another command pending handling in mainESP. The new command is discarded.";
        Serial.println(errorMsg);
        masterInstancePtr->debugSendLn(errorMsg);
        Serial.flush();
    }
    else{
        RIOCommandPending = true;
        RIOCommand = CAR_COMMAND;
        RIOArg1 = masterInstancePtr->myRIO.next();
        RIOArg2 = masterInstancePtr->myRIO.next();
        RIOArg3 = masterInstancePtr->myRIO.next();
    }
}

void AutoParkMaster::cartCommandHandler() {
    if(RIOCommandPending) {  // There is an unhandled command
        String errorMsg = "ERROR 10 There is another command pending handling in mainESP. The new cart command is discarded.";
        Serial.println(errorMsg);
        masterInstancePtr->debugSendLn(errorMsg);
        Serial.flush();
    }
    else{
        RIOCommandPending = true;
        RIOCommand = CART_COMMAND;
        RIOArg1 = masterInstancePtr->myRIO.next();
        RIOArg2 = masterInstancePtr->myRIO.next();
        RIOArg3 = masterInstancePtr->myRIO.next();
    }
}

void AutoParkMaster::unknownCommandHandler(const char *theCmd){
    String errorMsg = "ERROR 12 Unknown command: " + (String)theCmd;
    Serial.println(errorMsg);
    masterInstancePtr->debugSendLn(errorMsg);
}

void AutoParkMaster::handleRIOCommand() {
    myRIO.readSerial(); // Read serial commands
    if(RIOCommandPending) {
        if(RIOCommand == CAR_COMMAND) {
            int floorNum;
            int carIndex;
            int inOrOut;

            char *floorArg = RIOArg1;
            char *carArg = RIOArg2;
            char *inOutArg = RIOArg3;

            if(floorArg != NULL && carArg != NULL && inOutArg != NULL) {
                floorNum = atoi(floorArg);
                carIndex = atoi(carArg);
                inOrOut = atoi(inOutArg);
                // TODO: Send this to floor
                debugSendLn("Sending car cmd: Floor" + String(floorNum) + " Car:" + String(carIndex) + " InorOut" +
                            String(inOrOut));

                int floorIndex;
                switch(floorNum) {
                    case 0: floorIndex = FLOOR1; break;
                    case 1: floorIndex = FLOOR2; break;
                    case 2: floorIndex = FLOOR3; break;
                    default: Serial.println("ERROR 12 Unexpected Floor Index:" + (String)floorNum); return;
                }

                FloorArg2 operatingCar;
                switch(carIndex) {
                    case 0: operatingCar = FloorArg2::car1; break;
                    case 1: operatingCar = FloorArg2::car2; break;
                    case 2: operatingCar = FloorArg2::car3; break;
                    default: Serial.println("ERROR 13 Unexpected Car Index:" + (String) carIndex); return;
                }

                FloorCarDestination carTarget;
                switch(inOrOut){
                    case 0: carTarget = FloorCarDestination::toCart; break;
                    case 1: carTarget = FloorCarDestination::inElev; break;
                    case 2: carTarget = FloorCarDestination::inLot; break;
                    default: Serial.println("ERROR 14 Unexpected Car Target:" + (String) inOrOut); return;
                }

                send(macs[floorIndex], FloorCommand(FloorOperation::moveCar, operatingCar, carTarget), MSG_LEN);
            }
            else {  // Some arguments missing
                int emptyCommandCount = 0;
                if(floorArg == NULL) emptyCommandCount++;
                if(carArg == NULL) emptyCommandCount++;
                if(inOutArg == NULL) emptyCommandCount++;
                String ErrMsg = "ERROR 11 Incorrect command argument length for a Car command! Expected 3, received " + String(emptyCommandCount);
                Serial.println(ErrMsg);
                debugSendLn(ErrMsg);
            }

        }
        else { // It is a cart command
            int floorNum;
            int targetPosition;

            char *floorArg = RIOArg1;
            char *posArg = RIOArg2;

            if(floorArg != NULL && posArg != NULL) {
                floorNum = atoi(floorArg);
                targetPosition = atoi(posArg);
                // TODO: Send this to floor
                debugSendLn("Sending cart cmd: Floor" + String(floorNum) + " Moveto:" + String(3-targetPosition));

                int floorIndex;
                switch(floorNum) {
                    case 0: floorIndex = FLOOR1; break;
                    case 1: floorIndex = FLOOR2; break;
                    case 2: floorIndex = FLOOR3; break;
                    default: Serial.println("ERROR 12 Unexpected Floor Index:" + (String)floorNum); return;
                }

                FloorArg2 operatingCar;
                switch(targetPosition) {
                    case 0: operatingCar = FloorArg2::elevator; break;
                    case 1: operatingCar = FloorArg2::lot1; break;
                    case 2: operatingCar = FloorArg2::lot2; break;
                    case 3: operatingCar = FloorArg2::lot3; break;
                    default: Serial.println("ERROR 13 Unexpected Cart Destination:" + (String) targetPosition); return;
                }

                send(macs[floorIndex], FloorCommand(FloorOperation::moveCart, operatingCar), MSG_LEN);
            }
            else {  // Some arguments missing
                int emptyCommandCount = 0;
                if(floorArg == NULL) emptyCommandCount++;
                if(posArg == NULL) emptyCommandCount++;
                String ErrMsg = "ERROR 11 Incorrect command argument length for a Car command! Expected 2, received " + String(2-emptyCommandCount);
                Serial.println(ErrMsg);
                debugSendLn(ErrMsg);
            }
        }
        RIOCommandPending = false;
    }
}

