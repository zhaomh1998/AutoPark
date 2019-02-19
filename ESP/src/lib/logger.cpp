//
// Created by Scott Zhao on 2019-02-18.
//

#include "logger.h"


logger::logger() {
    pinMode(DEFAULT_LED, OUTPUT);
    digitalWrite(DEFAULT_LED, HIGH);
}

logger::logger(size_t serialBaud) {
    pinMode(DEFAULT_LED, OUTPUT);
    digitalWrite(DEFAULT_LED, HIGH);
    Serial.begin(serialBaud);
    Serial.println();
    logOption = 1;
}

void logger::error(String text) {
    digitalWrite(DEFAULT_LED, LOW);
    serialLog(text);
}

void logger::warning(String info) {
    ledState = !ledState;
    digitalWrite(DEFAULT_LED, ledState ? LOW : HIGH);
    serialLog(info);
}

void logger::info(String info) {
    serialLog(info);
}


void logger::serialLog(String text) {
    if (logOption == 1) {
        Serial.println(text);
    }
}