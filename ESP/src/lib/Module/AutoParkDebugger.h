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
#define ESPNOW_MSG_BUF_SIZE 512

extern bool messagePending;
extern uint8_t messageOrigin[6];
extern uint8_t messageData[ESPNOW_MSG_BUF_SIZE];
extern uint8_t messageLen;

class AutoParkDebugger : public AutoParkConfig{
public:


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
