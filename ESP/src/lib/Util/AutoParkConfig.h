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
    uint8_t *Ack(bool trueOrFalse) {
        theCmd[0] = 0x00;
        theCmd[1] = trueOrFalse ? 0x01 : 0x00;
        return theCmd;
    }

    // Floor command set
    enum class FloorOperation{statusUpdate, calibration, moveCart, moveCar};
    enum class FloorArg2{NoArg, request, ready, working, error, // Status Update Param
                        elevator, lot1, lot2, lot3, // MoveCart Param
                        car1, car2, car3}; // MoveCar Param
    enum class FloorCarDestination{NoArg, inLot, inElev, toCart};
    uint8_t *FloorCmd(FloorOperation anOp, FloorArg2 anArg2 = FloorArg2::NoArg,
            FloorCarDestination aDestination = FloorCarDestination::NoArg) {
        theCmd[0] = 0x01;
        switch(anOp) {
            case FloorOperation::statusUpdate:  theCmd[1] = 0x00; break;
            case FloorOperation::calibration:   theCmd[1] = 0x01; break;
            case FloorOperation::moveCart:      theCmd[1] = 0x02; break;
            case FloorOperation::moveCar:       theCmd[1] = 0x03; break;
        }
        switch(anArg2) {
            // For Status Update
            case FloorArg2::NoArg:    theCmd[2] = 0x00; break;
            case FloorArg2::request:  theCmd[2] = 0x01; break;
            case FloorArg2::ready:    theCmd[2] = 0x02; break;
            case FloorArg2::working:  theCmd[2] = 0x03; break;
            case FloorArg2::error:    theCmd[2] = 0x04; break;
            // For MoveCart
            case FloorArg2::elevator: theCmd[2] = 0x01; break;
            case FloorArg2::lot1:     theCmd[2] = 0x02; break;
            case FloorArg2::lot2:     theCmd[2] = 0x03; break;
            case FloorArg2::lot3:     theCmd[2] = 0x04; break;
            // For MoveCar
            case FloorArg2::car1:     theCmd[2] = 0x01; break;
            case FloorArg2::car2:     theCmd[2] = 0x02; break;
            case FloorArg2::car3:     theCmd[2] = 0x03; break;
        }
        switch(aDestination) {
            case FloorCarDestination::NoArg:  theCmd[3] = 0x00; break;
            case FloorCarDestination::inLot:  theCmd[3] = 0x00; break;
            case FloorCarDestination::inElev: theCmd[3] = 0x01; break;
            case FloorCarDestination::toCart: theCmd[3] = 0x02; break;
        }
        return theCmd;
    }

    // Car command set
    enum class CarCommand{forward, backward, stop};
    uint8_t *CarCmd(CarCommand aCommand) {
        switch (aCommand) {
            case CarCommand::forward:   theCmd[0] = 0x00; break;
            case CarCommand::backward:  theCmd[0] = 0x01; break;
            case CarCommand::stop:      theCmd[0] = 0x02; break;
        }
        return theCmd;
    }


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
            for (nthByte = 6; nthByte != UINT8_MAX; nthByte--) {
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

    int whoIsThisIndex(uint8_t *macAddr) {
        uint8_t nthIndex = 0;
        for (nthIndex = 0; nthIndex < macs.size(); nthIndex++) {
            uint8_t nthByte;
            for (nthByte = 6; nthByte != UINT8_MAX; nthByte--) {
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
