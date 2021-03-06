#include "logger.h"

void logger::log(const uint8_t &logType, const String &text) {
    switch (logType) {
        case READY:
            statusLED.ready();
            break;
        case PROCESSED:
            statusLED.processing();
            break;
        case WARNING:
            statusLED.warning();
            break;
        case ERROR:
            statusLED.error();
            break;
        default:
            statusLED.error();
    }
    if (isDebugMode) {
        switch (logType) {
            case READY:
                printStr("[READY]\t");
                break;
            case PROCESSED:
                printStr("[INFO]\t");
                break;
            case WARNING:
                printStr("[WARNING]\t");
                break;
            case ERROR:
                printStr("[ERROR]\t");
                break;
            default:
                printStr("[ERROR]\tUndefined log type!");
        }
        printStr(text);
    }
}

void logger::printStr(const String &aStr) {
    Serial.print(aStr);
}

void logger::printByte(const uint8_t &aByte) {
    Serial.printf("%x", aByte);
}

void logger::printMac(const uint8_t *macAddr) {
    Serial.printf("%2x:%2x:%2x:%2x:%2x:%2x", macAddr[0], macAddr[1], macAddr[2], macAddr[3], macAddr[4], macAddr[5]);
}

void logger::printESPNowMsg(bool dataType, uint8_t *mac, uint8_t *data, uint8_t &len) {
    printStr((dataType?"\t\t[<<TX]@":"\t [>>RX]@") + (String)millis()); printStr("\tPeer["); printByte(mac[5]); printStr("]\t" + (String)len + "Byte:");
    for(uint8_t nthByte = 0; nthByte < len; nthByte++) {
        printStr("["); printByte(data[nthByte]); printStr("]");
    }
    printStr("\n");
}

String logger::getByte(const uint8_t &aByte) {
    switch(aByte) {
        case 0x00: return "<00>";
        case 0x01: return "<01>";
        case 0x02: return "<02>";
        case 0x03: return "<03>";
        case 0x04: return "<04>";
        case 0x05: return "<05>";
        case 0x06: return "<06>";
        default: return "<OutOfRange>";
    }
}
void logger::assert(bool &result, String &errorMsg) {
    if(!result) {
        printStr("[AssertionERROR]\t" + errorMsg);
    }
}
