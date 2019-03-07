//
// Created by Scott Zhao on 2019-03-03.
//

#ifndef ESP_AUTOPARKMASTER_H
#define ESP_AUTOPARKMASTER_H
#ifndef ICACHE_FLASH
#define ICACHE_RAM_ATTR
#endif

#include "../lib/SerialCommand/SerialCommand.h"
#include "lib/Communication/master.h"
#define CAR_COMMAND 0
#define CART_COMMAND 1

#define FLOOR1_STEPPER_EN 12
#define FLOOR2_STEPPER_EN 13
#define FLOOR3_STEPPER_EN 14
#define STEPPER_ON 0
#define STEPPER_OFF 1

#define ELEVATOR_PHOTORESISTOR 16

enum class CommandStatus {
    APDisconnected,
    APCalibrating,
    APReady,
    APSent,
    APDone,
    APError
};

struct Floor {
    Floor(uint8_t *floorMac, int floorIndex) : macAddr(floorMac), index(floorIndex){}
    uint8_t *macAddr;
    int index;
    CommandStatus status = CommandStatus::APDisconnected;
};

class AutoParkMaster;
extern AutoParkMaster* masterInstancePtr;

class AutoParkMaster : public master{
public:
    AutoParkMaster(uint8_t myMacIndex, bool debug=false);

    // Serial command handlers
    static bool RIOCommandPending;
    static uint8_t RIOCommand;
    static char* RIOArg1;
    static char* RIOArg2;
    static char* RIOArg3;

    // UART Command handlers
    static void ICACHE_RAM_ATTR carCommandHandler();
    static void ICACHE_RAM_ATTR cartCommandHandler();
    static void ICACHE_RAM_ATTR unknownCommandHandler(const char *theCmd);
    void handleRIOCommand();

    // ESPNow Command handler
    bool messageHandler() override {  // override this method in subclasses
        if(!messagePending)
            return false;
        else {
            printESPNowMsg(RECEIVE, messageOrigin, messageData, messageLen);
            int sender = whoIsThisIndex(messageOrigin);
            switch(messageData[0]) {
                case TARGET_ACK:
                    if(sender == cmdTarget) {cmdACK = true; cmdACKResult = (messageData[1] == ACK_TRUE); }
                    else debugSendLn("ERROR 209 Unexpected ack sender:" + (String) sender);
                    break;

                case TARGET_FLOOR:
                    switch(messageData[1]) {
                        case FLOOR_STATUS_GET_ELEV_LASER: send(macs[sender], Ack((bool) digitalRead(ELEVATOR_PHOTORESISTOR)), MSG_LEN); break;
                        case FLOOR_STATUS_TURN_ON_STEPPER:
                            switch(sender) {
                                case FLOOR1: digitalWrite(FLOOR1_STEPPER_EN, STEPPER_ON); break;
                                case FLOOR2: digitalWrite(FLOOR2_STEPPER_EN, STEPPER_ON); break;
                                case FLOOR3: digitalWrite(FLOOR3_STEPPER_EN, STEPPER_ON); break;
                                default: debugSendLn("ERROR 211 Unexpected sender:" + (String) sender);
                            }
                            break;
                        case FLOOR_STATUS_TURN_OFF_STEPPER:
                            switch(sender) {
                                case FLOOR1: digitalWrite(FLOOR1_STEPPER_EN, STEPPER_OFF); break;
                                case FLOOR2: digitalWrite(FLOOR2_STEPPER_EN, STEPPER_OFF); break;
                                case FLOOR3: digitalWrite(FLOOR3_STEPPER_EN, STEPPER_OFF); break;
                                default: debugSendLn("ERROR 212 Unexpected sender:" + (String) sender);
                            }
                            break;
                    }
                default:
                    debugSendLn("ERROR 210 Unexpected MSG Target" + getByte(messageData[0]));
            }
//            for(Floor nthFloor : floorList) {
//                if(nthFloor.index == sender) {
//                    floorMessageHandler(nthFloor, messageData, messageLen);
//                }
//            }
            messagePending = false;
            return true;
        }
    }

    bool floorMessageHandler(Floor aFloor, uint8_t *aMessage, uint8_t &messageLen) {
        debugSendLn("Message from floor!");
        return true;
    }
    // Util for init
    void addFloor(int floorIndex) {
        floorList.emplace_back(Floor(macs[floorIndex], floorIndex));
        // TODO: Send calibration
    }
    bool initFloors(uint8_t floorIndex) {
        return true;
    }
    bool areFloorReady();


protected:
    std::vector <Floor> floorList;
    SerialCommand myRIO;
    int cmdTarget;
    bool cmdACK;
    bool cmdACKResult;
};




#endif //ESP_AUTOPARKMASTER_H
