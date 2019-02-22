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

extern "C" {
#include <espnow.h>
#include "user_interface.h"
}

#define WIFI_CHANNEL 4

#include "lib/Util/logger.h"

class ESPNow {
public:
    explicit ESPNow(uint8_t *myMacAddr, bool debug);

    static void messageHandler(uint8_t *mac, uint8_t *data, uint8_t len);

    virtual void add_peer(uint8_t *mac, u8 channel = WIFI_CHANNEL) {
        esp_now_add_peer(mac, ESP_NOW_ROLE_SLAVE, channel, nullptr, 0);
    }

    void send(uint8_t *mac, uint8_t *msg, int len) {
        esp_now_send(mac, msg, len);
    }


protected:
    logger logHandle;
};


#endif