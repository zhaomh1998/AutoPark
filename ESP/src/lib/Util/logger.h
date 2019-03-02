////
//// Created by Scott Zhao on 2019-02-18.
////
//
//#ifndef ESP_LOG_H
//#define ESP_LOG_H
//// For data modes
//#define SEND true
//#define RECEIVE false
//
//#include "RGB.h"
//#include "AutoParkConfig.h"
//
//class logger : public AutoParkConfig {
//public:
//    logger(size_t serialBaud, bool debug = false) {
//        Serial.begin(115200);
//    }
//
//    void debug(String msg) {}
//
//    void info(String msg) {}
//
//    void error(String msg) {}
//
//    void printESPNowMsg(bool dataType, uint8_t *mac, uint8_t *data, uint8_t len) {
//        serialLogStatic("*[DEBUG]\t");
//        if (dataType)
//            serialLogStatic("Sent to [");
//        else
//            serialLogStatic("Recv from [");
//        // TODO: Is there a better way?
//        AutoParkConfig AP;
//        serialLogStatic(AP.whoIsThis(mac));
//        serialLogStatic("]\tData Length[");
//        serialLogStatic(String(len));
//        serialLogStatic("]\tData->");
//        for (int dataIndex = 0; dataIndex < len; dataIndex++)
//            serialLogByteStatic(data[dataIndex]);
//        serialLogStatic("<-\n");
//    }
//
//    void serialLogStatic(String text) {
//        Serial.print(text);
//    }
//
//    void serialLogByteStatic(uint8_t singleByte) {
//        Serial.printf("%x", singleByte);
//    }
//};
//
//
//#endif //ESP_LOG_H













//
// Created by Scott Zhao on 2019-02-18.
//

#ifndef ESP_LOG_H
#define ESP_LOG_H
// For data modes
#define SEND true
#define RECEIVE false

#include "RGB.h"
#include "AutoParkConfig.h"

#define DEFAULT_LED 2

class logger : public AutoParkConfig {
public:
    logger();

    logger(size_t serialBaud, bool debug = false);

    void error(String text);

    void warning(String info);

    void info(String info);

    void debug(String info);

    void assert(bool &result, String errorMsg);

    void debugESPNowMsg(bool dataType, uint8_t *mac, uint8_t *data, uint8_t len);

    static void printESPNowMsg(bool dataType, uint8_t *mac, uint8_t *data, uint8_t len);

    void serialLogln(String text);

    void serialLog(String text);

    static void serialLogStatic(String text);

    void serialLogByte(uint8_t singleByte);

    static void serialLogByteStatic(uint8_t singleByte);

    static void printMac(uint8_t *macAddr);

private:
    int logOption; // 0 - built in led only, 1 - built in led + serial
    bool ledState; // true - on, false - off
    bool debugMode;
};


#endif //ESP_LOG_H
