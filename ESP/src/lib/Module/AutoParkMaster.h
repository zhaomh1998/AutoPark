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
//            printESPNowMsg(RECEIVE, messageOrigin, messageData, messageLen);
//            int sender = whoIsThisIndex(messageOrigin);
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
};




#endif //ESP_AUTOPARKMASTER_H
