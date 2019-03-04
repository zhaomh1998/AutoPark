//
// Created by Scott Zhao on 2019-03-03.
//

#ifndef ESP_AUTOPARKDEBUGGER_H
#define ESP_AUTOPARKDEBUGGER_H
#define WIFI_CHANNEL 4

#ifndef ICACHE_FLASH
#define ICACHE_RAM_ATTR
#endif

#include <lib/Util/AutoParkConfig.h>
#include <ESP8266WiFi.h>
#include <espnow.h>


class AutoParkDebugger : public AutoParkConfig{
public:
    static bool messagePending;
    static uint8_t *messageOrigin;
    static uint8_t *messageData;
    static uint8_t messageLen;

    explicit AutoParkDebugger(uint8_t debuggerIndex);

    static void ICACHE_RAM_ATTR msgCallback(uint8_t *mac, uint8_t *data, uint8_t len);

    void handle() {
        if (messagePending) {
            Serial.print((char *) messageData);
            messagePending = false;
        }
    }
};


#endif //ESP_AUTOPARKDEBUGGER_H
