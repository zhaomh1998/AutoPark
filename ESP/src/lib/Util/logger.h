//
// Created by Scott Zhao on 2019-02-18.
//

#ifndef ESP_LOG_H
#define ESP_LOG_H
// For data modes
#define SEND true
#define RECEIVE false

#include <Arduino.h>
//#include "AutoParkConfig.h"

#define DEFAULT_LED 2

class logger {
public:
    logger();

    logger(size_t serialBaud, bool debug = false);

    void error(String text);

    void warning(String info);

    void info(String info);

    void debug(String info);

    void debugESPNowMsg(bool dataType, uint8_t *mac, uint8_t *data, uint8_t len);

    void serialLogln(String text);

    void serialLog(String text);

    void serialLogByte(uint8_t singleByte);

private:
    int logOption = 0; // 0 - built in led only, 1 - built in led + serial
    bool ledState = true; // true - on, false - off
    bool debugMode = false;
};


#endif //ESP_LOG_H
