//
// Created by Scott Zhao on 2019-02-18.
//

#include "ESPNow.h"

bool messagePending;
uint8_t messageOrigin[6];
uint8_t messageData[ESPNOW_MSG_BUF_SIZE];
uint8_t messageLen;

ESPNow::ESPNow(uint8_t deviceName, bool debug = false) : logger(debug), isDebugMode(debug) {
    myName = whoIsThis(macs[deviceName]);
    log(PROCESSED, "\nSetting up ESPNow. My MAC: ");
    WiFi.mode(WIFI_AP);
    wifi_set_macaddr(SOFTAP_IF, macs[deviceName]);
    log(PROCESSED, WiFi.softAPmacAddress() + "\n");
    if (esp_now_init() != 0) {
        log(ERROR, "*** ESP_Now init failed\n");
        ESP.restart();
    }

    esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
    messagePending = false;
    esp_now_register_recv_cb(msgCallback);
    if(debug) addPeer(DEBUGGER);
    debugSendLn("Initialized");

}

void ESPNow::msgCallback(uint8_t *mac, uint8_t *data, uint8_t len) {
    messagePending = true;
    Serial.println(messagePending);
    // TODO: did I copy the content?
//    Serial.printf("\nReceived from %x:%x:%x:%x:%x:%x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    for(int nthByte = 0; nthByte < 6; nthByte++) {
        messageOrigin[nthByte] = mac[nthByte];
    }

    for(int nthByte = 0; nthByte < len; nthByte++) {
        messageData[nthByte] = data[nthByte];
    }
//    messageOrigin = mac;
//    messageData = data;
    messageLen = len;
}