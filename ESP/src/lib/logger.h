//
// Created by Scott Zhao on 2019-02-18.
//

#ifndef ESP_LOG_H
#define ESP_LOG_H

#include <Arduino.h>

#define DEFAULT_LED 2

class logger {
public:
    logger();

    explicit logger(size_t serialBaud);

    void error(String text);

    void warning(String info);

    void info(String info);

    void serialLog(String text);

private:
    int logOption = 0; // 0 - built in led only, 1 - built in led + serial
    bool ledState = true; // true - on, false - off
};


#endif //ESP_LOG_H
