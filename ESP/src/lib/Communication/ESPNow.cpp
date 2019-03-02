//
// Created by Scott Zhao on 2019-02-18.
//

#include "ESPNow.h"

ESPNow::ESPNow(uint8_t deviceName, bool debug = false) : logger(115200, debug), isDebugMode(debug) {
    info("Setting up ESPNow. My MAC:");
    WiFi.mode(WIFI_AP);
    wifi_set_macaddr(SOFTAP_IF, macs[deviceName]);
    info(WiFi.softAPmacAddress());
    if (esp_now_init() != 0) {
        error("*** ESP_Now init failed");
        ESP.restart();
    }

    esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
    setMsgCallback();
}

//void ESPNow::messageHandler(uint8_t *mac, uint8_t *data, uint8_t len) {
//    Serial.printf("Message from %02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
//    Serial.print(", Length: ");
//    Serial.print(len);
//    Serial.print(", Message.");
//    for (int nthByte = 0; nthByte < len; nthByte++)
//        Serial.printf("%x", data[nthByte]);
//    Serial.println();
//}
//

void ESPNow::messageHandlerDebug(uint8_t *mac, uint8_t *data, uint8_t len) {
    logger::printESPNowMsg(RECEIVE, mac, data, len);
}