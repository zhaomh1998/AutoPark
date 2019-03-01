//
// Created by Scott Zhao on 2019-02-21.
//
#ifndef ESP_AUTOPARKCONFIG_H
#define ESP_AUTOPARKCONFIG_H

#include <Arduino.h>
#include <vector>
#include <algorithm>
#include "logger.h"

#define MASTER 0
#define FLOOR1 1
#define FLOOR2 2
#define FLOOR3 3
#define CAR1 4
#define CAR2 5
#define CAR3 6

extern uint8_t MAC_Master[];
extern uint8_t MAC_Level1[];
extern uint8_t MAC_Level2[];
extern uint8_t MAC_Level3[];
extern uint8_t MAC_Car1[];
extern uint8_t MAC_Car2[];
extern uint8_t MAC_Car3[];
const std::vector<uint8_t *> macs({MAC_Master, MAC_Level1, MAC_Level2, MAC_Level3,
                                  MAC_Car1, MAC_Car2, MAC_Car3});
const std::vector<String> deviceNames{"Master", "Level1", "Level2", "Level3", "Car1", "Car2", "Car3"};

extern String whoIsThis(uint8_t *macAddr);

#endif //ESP_AUTOPARKCONFIG_H
