//
// Created by Scott Zhao on 2019-02-21.
//
#ifndef ESP_AUTOPARKCONFIG_H
#define ESP_AUTOPARKCONFIG_H

#include <Arduino.h>
#include <vector>
#include <algorithm>

#define MASTER 0
#define FLOOR1 1
#define FLOOR2 2
#define FLOOR3 3
#define CAR1 4
#define CAR2 5
#define CAR3 6
#define DEBUGGER 7

// Command set targets
#define MSG_LEN 4
#define TARGET_ACK 0x00
#define TARGET_FLOOR 0x01
#define TARGET_CAR 0x02

// --- Ack command defs
#define ACK_FALSE 0x00
#define ACK_TRUE 0x01


// --- Floor command Defs
// Op
#define FLOOR_STATUS_UPDATE_CMD 0x00
#define FLOOR_CALIBRATE_CMD 0x01
#define FLOOR_MOVE_CART_CMD 0x02
#define FLOOR_MOVE_CAR_CMD 0x03

#define FLOOR_NO_ARG 0x00

// For floor commands
#define FLOOR_STATUS_REQUEST 0x00
#define FLOOR_STATUS_READY 0x01
#define FLOOR_STATUS_BUSY 0x02
#define FLOOR_STATUS_ERROR 0x03
#define FLOOR_STATUS_GET_ELEV_LASER 0x03
// For MoveCart
#define FLOOR_ELEVATOR 0x00
#define FLOOR_LOT1 0x01
#define FLOOR_LOT2 0x02
#define FLOOR_LOT3 0x03
// For MoveCar
#define FLOOR_CAR1 0x00
#define FLOOR_CAR2 0x01
#define FLOOR_CAR3 0x02
// Car command set
#define FLOOR_CAR_IN_LOT 0x00
#define FLOOR_CAR_IN_ELEV 0x01
#define FLOOR_CAR_TO_CART 0x02

// --- Car command defs
#define CAR_FORWARD 0x00
#define CAR_BACKWARD 0x01
#define CAR_STOP 0x02


class AutoParkConfig {
public:
    uint8_t MAC_Master[6] = {0x36, 0x00, 0x00, 0x00, 0x66, 0x33};
    uint8_t MAC_Level1[6] = {0x36, 0x00, 0x00, 0x00, 0x66, 0x34};
    uint8_t MAC_Level2[6] = {0x36, 0x00, 0x00, 0x00, 0x66, 0x35};
    uint8_t MAC_Level3[6] = {0x36, 0x00, 0x00, 0x00, 0x66, 0x36};
    uint8_t MAC_Car1[6] = {0x36, 0x00, 0x00, 0x00, 0x66, 0x37};
    uint8_t MAC_Car2[6] = {0x36, 0x00, 0x00, 0x00, 0x66, 0x38};
    uint8_t MAC_Car3[6] = {0x36, 0x00, 0x00, 0x00, 0x66, 0x39};
    uint8_t MAC_Debugger[6] = {0x36, 0x00, 0x00, 0x00, 0x66, 0x40};

    // Common Command Sets
    // Target : Op : Param1(optional) : Param 2(optional)
    static uint8_t theCmd[4];

    // Ack command set
    uint8_t *Ack(bool trueOrFalse);

    // Floor command set
    enum class FloorOperation{statusUpdate, calibration, moveCart, moveCar};
    enum class FloorArg2{NoArg, request, ready, working, error, getElevatorLaser, // Status Update Param
                        elevator, lot1, lot2, lot3, // MoveCart Param
                        car1, car2, car3}; // MoveCar Param
    enum class FloorCarDestination{NoArg, inLot, inElev, toCart};
    uint8_t *FloorCommand(FloorOperation anOp, FloorArg2 anArg2 = FloorArg2::NoArg,
                          FloorCarDestination aDestination = FloorCarDestination::NoArg);


    // Car command set
    enum class CarCommand{forward, backward, stop};
    uint8_t *CarCmd(CarCommand aCommand);


    std::vector<uint8_t *> macs;
    std::vector<String> deviceNames;

    AutoParkConfig() {
        macs = std::vector<uint8_t *>({MAC_Master, MAC_Level1, MAC_Level2, MAC_Level3, MAC_Car1, MAC_Car2, MAC_Car3, MAC_Debugger});
        deviceNames = std::vector<String>({"Master", "Level1", "Level2", "Level3", "Car1", "Car2", "Car3", "Debugger"});
    }


    String whoIsThis(uint8_t *macAddr) {
//        auto it = std::find_if(macs.begin(), macs.end(),
//                               [&macAddr](const uint8_t *nthMac) { return *nthMac == *macAddr; });
//        if (it == macs.end()) {
//            return "UNKNOWN MAC";
//        } else {
//            auto index = std::distance(macs.begin(), it);
//            Serial.println("Found index" + (String) index);
//            return deviceNames[index];
//        }
        uint8_t nthIndex = 0;
        for (nthIndex = 0; nthIndex < macs.size(); nthIndex++) {
            uint8_t nthByte;
            for (nthByte = 5; nthByte != UINT8_MAX; nthByte--) {
                // For each byte
                if (macs[nthIndex][nthByte] == macAddr[nthByte]) {
                    continue;
                } else
                    break;
            }
            if (nthByte == UINT8_MAX) { // Found
                break;
            }
        }
        if (nthIndex == macs.size())
            return "UNKNOWN MAC";
        return deviceNames[nthIndex];
    }

    int whoIsThisIndex(const uint8_t* macAddr) {
        uint8_t nthIndex = 0;
        for (nthIndex = 0; nthIndex < macs.size(); nthIndex++) {
            uint8_t nthByte;
            for (nthByte = 5; nthByte != UINT8_MAX; nthByte--) {
                // For each byte
                Serial.printf("%x - %x\n", macs[nthIndex][nthByte], macAddr[nthByte]);
                if (macs[nthIndex][nthByte] == macAddr[nthByte]) {
                    continue;
                } else
                    break;
            }
            if (nthByte == UINT8_MAX) { // Found
                break;
            }
        }
        if (nthIndex == macs.size())
            return -1;
        return nthIndex;
    }

    void printMac(uint8 *macAddr) {
        for (uint8_t macIndex = 0; macIndex < 6; macIndex++) {
            Serial.printf("%x ", macAddr[macIndex]);
        }
    }
};


#endif //ESP_AUTOPARKCONFIG_H
