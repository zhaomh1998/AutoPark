#ifndef ESP_LOG_H
#define ESP_LOG_H
#define SERIAL_BAUD_RATE 115200
#define STATUS_LED_PIN 15
#define SEND true
#define RECEIVE false
#define READY 1  // Light up solid green light
#define PROCESSED 2  // Toggles brightness on a yellow light
#define WARNING 3  // Orange light
#define ERROR 4  // Red light

#include "AutoParkConfig.h"
#include "RGB.h"

class logger : public AutoParkConfig {
public:
    explicit logger(bool isDebugMsgPrinted) : AutoParkConfig(), statusLED(STATUS_LED_PIN), isDebugMode(isDebugMsgPrinted)  {
        if (!Serial)
            Serial.begin(SERIAL_BAUD_RATE);
    }

    void log(const uint8_t &logType, const String &text);
    static void printStr(const String &aStr);
    static void printByte(const uint8_t &aByte);
    static void printMac(const uint8_t *macAddr);
    static void printESPNowMsg(bool dataType, uint8_t *mac, uint8_t *data, uint8_t &len);
    static void assert(bool &result, String &errorMsg);
    static String getByte(const uint8_t &aByte);

    RGB statusLED;

private:
    bool isDebugMode;
};

#endif //ESP_LOG_H
