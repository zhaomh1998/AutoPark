//
// Created by Scott Zhao on 2019-02-18.
//

#include "logger.h"


logger::logger() {
    pinMode(DEFAULT_LED, OUTPUT);
    digitalWrite(DEFAULT_LED, HIGH);
}

logger::logger(size_t serialBaud, bool debug) : debugMode(debug) {
    pinMode(DEFAULT_LED, OUTPUT);
    digitalWrite(DEFAULT_LED, HIGH);
    Serial.begin(serialBaud);
    Serial.println();
    logOption = 1;
}

void logger::error(String text) {
    digitalWrite(DEFAULT_LED, LOW);
    serialLog("[ERROR]\t");
    serialLogln(text);
}

void logger::warning(String info) {
    ledState = !ledState;
    digitalWrite(DEFAULT_LED, ledState ? LOW : HIGH);
    serialLog("[WARNING]\t");
    serialLogln(info);
}

void logger::info(String info) {
    serialLog("[INFO]\t");
    serialLogln(info);
}

void logger::debug(String info) {
    if (debugMode) {
        serialLog("*[DEBUG]\t");
        serialLogln(info);
    }
}

void logger::serialLogln(String text) {
    if (logOption == 1) {
        Serial.println(text);
    }
}

void logger::serialLog(String text) {
    if (logOption == 1) {
        Serial.print(text);
    }
}
