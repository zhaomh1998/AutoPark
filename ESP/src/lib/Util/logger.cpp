//
// Created by Scott Zhao on 2019-02-18.
//

#include "logger.h"
#include "AutoParkConfig.h"

logger::logger() : AutoParkConfig() {
    pinMode(DEFAULT_LED, OUTPUT);
    digitalWrite(DEFAULT_LED, HIGH);
    logOption = 0;
    ledState = true;
    debugMode = true;
}

logger::logger(size_t serialBaud, bool debug) : debugMode(debug), AutoParkConfig() {
    pinMode(DEFAULT_LED, OUTPUT);
    digitalWrite(DEFAULT_LED, HIGH);
    Serial.begin(serialBaud);
    Serial.println();
    logOption = 1;
    ledState = true;
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
    Serial.println("*enter");
    serialLog("[INFO]\t");
    serialLogln(info);
}

void logger::debug(String info) {
    if (debugMode) {
        serialLog("*[DEBUG] " + (String) millis() + "\t");
        serialLogln(info);
    }
}

void logger::debugESPNowMsg(bool dataType, uint8_t *mac, uint8_t *data, uint8_t len) {
    if (debugMode) {
        printESPNowMsg(dataType, mac, data, len);
    }
}

void logger::printESPNowMsg(bool dataType, uint8_t *mac, uint8_t *data, uint8_t len) {
    serialLogStatic("*[DEBUG]\t");
    if (dataType)
        serialLogStatic("Sent to [");
    else
        serialLogStatic("Recv from [");
    // TODO: Is there a better way?
    AutoParkConfig AP;
    serialLogStatic(AP.whoIsThis(mac));
    serialLogStatic("]\tData Length[");
    serialLogStatic(String(len));
    serialLogStatic("]\tData->");
    for (int dataIndex = 0; dataIndex < len; dataIndex++)
        serialLogByteStatic(data[dataIndex]);
    serialLogStatic("<-\n");
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

void logger::serialLogStatic(String text) {
    Serial.print(text);
}

void logger::serialLogByte(uint8_t singleByte) {
    if (logOption == 1) {
        Serial.printf("%x", singleByte);
    }
}

void logger::serialLogByteStatic(uint8_t singleByte) {
    Serial.printf("%x", singleByte);
}

void logger::assert(bool &result, String errorMsg) {
    if (!result) {
        error("[AssertionError] " + errorMsg);
    }
}

void logger::printMac(uint8_t *macAddr) {
    for (uint8_t macIndex = 0; macIndex < 6; macIndex++) {
        Serial.printf("%x ", macAddr[macIndex]);
    }
}