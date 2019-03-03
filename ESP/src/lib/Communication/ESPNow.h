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

class ESPNow : public logger {
public:
    ESPNow(uint8_t deviceName, bool debug);

    // https://arduino-esp8266.readthedocs.io/en/latest/faq/a02-my-esp-crashes.html#other-causes-for-crashes
    // https://github.com/esp8266/Arduino/issues/4468
    // https://github.com/PaulStoffregen/Encoder/pull/15
    static void ICACHE_RAM_ATTR messageHandlerDebug(uint8_t *mac, uint8_t *data, uint8_t len);

    virtual void addPeer(uint8_t deviceName) {
        setPeerMac(macs[deviceName], WIFI_CHANNEL);
    }

    void setPeerMac(uint8_t *mac, u8 channel) {
        esp_now_add_peer(mac, ESP_NOW_ROLE_SLAVE, channel, nullptr, 0);
        log(PROCESSED, "Added Peer " + whoIsThis(mac));
    }

    // Overload this to change the static messageHandler function
    virtual void setMsgCallback() {
        esp_now_register_recv_cb(messageHandlerDebug);
    }

    void send(uint8_t *mac, uint8_t *msg, uint8_t len) {
        if(isDebugMode)
            printESPNowMsg(SEND, mac, msg, len);
        esp_now_send(mac, msg, len);
    }


protected:
    bool isDebugMode;
};


#endif
