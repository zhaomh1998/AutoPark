//
// Created by Scott Zhao on 2019-03-03.
//

#include "AutoParkDebugger.h"

bool AutoParkDebugger::messagePending;
uint8_t *AutoParkDebugger::messageOrigin;
uint8_t *AutoParkDebugger::messageData;
uint8_t AutoParkDebugger::messageLen;

AutoParkDebugger::AutoParkDebugger(uint8_t debuggerIndex) : AutoParkConfig() {
    Serial.begin(115200);
    Serial.println();
    // Init ESP Now
    WiFi.mode(WIFI_AP);
    wifi_set_macaddr(SOFTAP_IF, macs[debuggerIndex]);
    if (esp_now_init() != 0) {
        Serial.println("[ERROR] ESP_Now init failed\n");
        ESP.restart();
    }
    esp_now_set_self_role(ESP_NOW_ROLE_COMBO);

    // Add peers
    size_t deviceListLength = macs.size();
    for (uint8_t currentIndex = 0; currentIndex < deviceListLength; currentIndex++) {
        if (currentIndex == debuggerIndex)
            continue;  // skip the exclude index
        esp_now_add_peer(macs[currentIndex], ESP_NOW_ROLE_SLAVE, WIFI_CHANNEL, nullptr, 0);
        Serial.println("[INFO]" + whoIsThis(macs[currentIndex]) + " Peer Added!");
    }

    // Set msg callback
    esp_now_register_recv_cb(msgCallback);
}

void ICACHE_RAM_ATTR AutoParkDebugger::msgCallback(uint8_t *mac, uint8_t *data, uint8_t len) {
    messagePending = true;
    // TODO: did I copy the content?
    messageOrigin = mac;
    messageData = data;
    messageLen = len;
}
