/**
   ESP8266 ESP-NOW Base Class
   Created by Scott Zhao 2/18/2019

   This sketch is heavily based on
      Anthony Elder's work: https://github.com/HarringayMakerSpace/ESP-Now
      Andreas Spiess's work: https://github.com/SensorsIot/ESP-Now-Tests
*/


#ifndef ESPNOW_H
#define ESPNOW_H
#ifndef ESP8266
#pragma message("This library only works on ESP8266")
#endif

#include <ESP8266WiFi.h>
#include "../Util/AutoParkConfig.h"

extern "C" {
#include <espnow.h>
#include "user_interface.h"
}

#ifndef ICACHE_FLASH
#define ICACHE_RAM_ATTR
#endif

#define WIFI_CHANNEL 4

#include "lib/Util/logger.h"

#define ESPNOW_MSG_BUF_SIZE 512
extern bool messagePending;
extern uint8_t messageOrigin[6];
extern uint8_t messageData[ESPNOW_MSG_BUF_SIZE];
extern uint8_t messageLen;

class ESPNow : public logger {
public:

    ESPNow(uint8_t deviceName, bool debug);
    // Added ICACHE_RAM_ATTR, see the following
    // https://arduino-esp8266.readthedocs.io/en/latest/faq/a02-my-esp-crashes.html#other-causes-for-crashes
    // https://github.com/esp8266/Arduino/issues/4468
    // https://github.com/PaulStoffregen/Encoder/pull/15
    static void ICACHE_RAM_ATTR msgCallback(uint8_t *mac, uint8_t *data, uint8_t len);

    virtual void addPeer(uint8_t deviceName) {
        setPeerMac(macs[deviceName], WIFI_CHANNEL);
    }

    void setPeerMac(uint8_t *mac, u8 channel) {
        esp_now_add_peer(mac, ESP_NOW_ROLE_SLAVE, channel, nullptr, 0);
        log(PROCESSED, "Added Peer " + whoIsThis(mac) + "\n");
    }

    void send(uint8_t *mac, uint8_t *msg, uint8_t len) {
        if(isDebugMode)
            printESPNowMsg(SEND, mac, msg, len);
        esp_now_send(mac, msg, len);
    }

    virtual bool messageHandler() {  // override this method in subclasses
        if(!messagePending)
            return false;
        else {
            if(isDebugMode)
                printESPNowMsg(RECEIVE, messageOrigin, messageData, messageLen);
            messagePending = false;
            return true;
        }
    }

    void debugSendLn(const String &debugMsg) {
        String msg = "[" + myName + "]\t" + debugMsg + "\n";
        uint8 buf[msg.length() + 1];
        msg.getBytes(buf, msg.length() + 1);
        send(macs[DEBUGGER], buf, msg.length() + 1);
    }

    void debugSend(const String &debugMsg) {
        uint8 buf[debugMsg.length() + 1];
        debugMsg.getBytes(buf, debugMsg.length() + 1);
        send(macs[DEBUGGER], buf, debugMsg.length() + 1);
    }

    bool debugAssert(bool result, String errorMsg) {
        if(!result) {
            String message = "[AssertionERROR]\t" + errorMsg;
            printStr(message);
            debugSendLn(message);
            return false;
        }
        return true;
    }



protected:
    bool isDebugMode;
    String myName;
};


#endif
