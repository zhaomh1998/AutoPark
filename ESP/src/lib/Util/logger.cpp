//
// Created by Scott Zhao on 2019-02-18.
//

#include "logger.h"
#include "AutoParkConfig.h"

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

void logger::debugESPNowMsg(bool dataType, uint8_t *mac, uint8_t *data, uint8_t len) {
    if (debugMode) {
        serialLog("*[DEBUG]\t");
        if (dataType)
            serialLog("Sent to [");
        else
            serialLog("Recv from [");
        serialLog(whoIsThis(mac));
        serialLog("]\tData Length[");
        serialLog(String(len));
        serialLog("]\tData->");
        for (int dataIndex = 0; dataIndex < len; dataIndex++)
            serialLogByte(data[dataIndex]);
        serialLogln("<-");
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

void logger::serialLogByte(uint8_t singleByte) {
    if (logOption == 1) {
        Serial.printf("%x", singleByte);
    }
}
