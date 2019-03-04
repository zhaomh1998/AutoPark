////
//// Created by Scott Zhao on 2019-03-03.
////
//
//#ifndef ESP_AUTOPARKMASTER_H
//#define ESP_AUTOPARKMASTER_H
//
//#include "../lib/SerialCommand/SerialCommand.h"
//#include "lib/Communication/master.h"
//
//enum class CommandStatus {
//    APDisconnected,
//    APCalibrating,
//    APReady,
//    APSent,
//    APDone,
//    APError
//};
//
//struct Floor {
//    Floor(uint8_t *floorMac, int floorIndex) : macAddr(floorMac), index(floorIndex){}
//    uint8_t *macAddr;
//    int index;
//    CommandStatus status = CommandStatus::APDisconnected;
//};
//
//class AutoParkMaster : public master{
//public:
//    AutoParkMaster(uint8_t myMacIndex, bool debug=false) : master(myMacIndex, debug), myRIO() {
//        statusLED.processing();
//        // Add floors
//        addFloor(FLOOR1);
//
//        // Initialize myRIO
//        myRIO.addCommand("CAR", carCommandHandler);
//        myRIO.addCommand("CART", cartCommandHandler);
//    }
//
//    // Serial command handlers
//    static bool RIOCommandPending;
//    static bool RIOCommand;
//    static uint8_t *messageData;
//    static uint8_t messageLen;
//
//    static void carCommandHandler();
//    static void cartCommandHandler();
//    void handleRIOCommand();
//
//    bool messageHandler() override {  // override this method in subclasses
//        if(!messagePending)
//            return false;
//        else {
//            printESPNowMsg(RECEIVE, messageOrigin, messageData, messageLen);
//            int sender = whoIsThisIndex(messageOrigin);
//            for(Floor nthFloor : floorList) {
//                if(nthFloor.index == sender) {
//                    floorMessageHandler(nthFloor, messageData, messageLen);
//                }
//            }
//            messagePending = false;
//            return true;
//        }
//    }
//
//    bool floorMessageHandler(Floor aFloor, uint8_t *aMessage, uint8_t &messageLen);
//    // Util for init
//    bool addFloor(int floorIndex) {
//        floorList.emplace_back(Floor(macs[floorIndex], floorIndex));
//    }
//    bool initFloors(uint8_t floorIndex);
//    bool areFloorReady();
//
//
//protected:
//    std::vector <Floor> floorList;
//    SerialCommand myRIO;
//};
//
//
//#endif //ESP_AUTOPARKMASTER_H
