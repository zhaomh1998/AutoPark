//
// Created by Scott Zhao on 2019-02-18.
//

#include "ESPNow.h"

bool ESPNow::messagePending;
uint8_t *ESPNow::messageOrigin;
uint8_t *ESPNow::messageData;
uint8_t ESPNow::messageLen;

ESPNow::ESPNow(uint8_t deviceName, bool debug = false) : logger(debug), isDebugMode(debug) {
    log(PROCESSED, "Setting up ESPNow. My MAC:");
    WiFi.mode(WIFI_AP);
    wifi_set_macaddr(SOFTAP_IF, macs[deviceName]);
    log(PROCESSED, WiFi.softAPmacAddress());
    if (esp_now_init() != 0) {
        log(ERROR, "*** ESP_Now init failed");
        ESP.restart();
    }

    esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
    messagePending = false;
    esp_now_register_recv_cb(msgCallback);
}

void ESPNow::msgCallback(uint8_t *mac, uint8_t *data, uint8_t len) {
    messagePending = true;
    Serial.println(messagePending);
    // TODO: did I copy the content?
    messageOrigin = mac;
    messageData = data;
    messageLen = len;

}