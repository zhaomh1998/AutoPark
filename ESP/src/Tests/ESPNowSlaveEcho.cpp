//#include <ESP8266WiFi.h>
//
//extern "C" {
//#include <espnow.h>
//#include "user_interface.h"
//}
//
//#define WIFI_CHANNEL 4
//
//uint8_t MASTER_MAC[] = {0x36, 0x00, 0x00, 0x00, 0x66, 0x33};
//uint8_t SELF_MAC[] = {0x36, 0x00, 0x00, 0x00, 0x66, 0x37};
//int lastTime;
//
///**
//   @brief     Send bytes through ESP Now to target mac address
//
//   @param     mac  MAC Address to send message to
//   @param     msg  Bytes array containing the data to send
//*/
//void sendEspNow(uint8_t *mac, uint8_t *msg, uint8_t msgLen) {
//    Serial.printf("Sending to %02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
//    Serial.print(": ->");
//    for(int nthByte = 0; nthByte < msgLen; nthByte++) {
//        Serial.printf(" %x ",msgLen);
//    }
//    Serial.println("<-");
//    esp_now_send(mac, msg, msgLen); // set mac parameter to NULL to send to all devices
//}
//
///**
//   @brief     Handles message receive event for ESP_Now
//
//   @param     msgSourceMac  Message source's MAC Addr in bytes array
//   @param     msg  Content of the message in bytes array
//   @param     msgLen  Length of the message
//*/
//void echo(uint8_t *msgSourceMac, uint8_t *msg, uint8_t msgLen) {
//    Serial.print("Received Message Length:");
//    Serial.print(msgLen);
//    Serial.print("Message->");
//    Serial.write(msg, msgLen);
//    Serial.println("<-");
//    sendEspNow(msgSourceMac, msg, msgLen); //Echo msg back
//}
//
///**
//   @brief     Initialize ESP Now as both the master and slave
//*/
//void initEspNow() {
//    if (esp_now_init() != 0) {
//        Serial.println("*** ESP_Now init failed");
//        ESP.restart();
//    }
//
//    esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
//    esp_now_register_recv_cb([](uint8_t *mac, uint8_t *data, uint8_t len) {  // Initialize as receiver
//
//        // Investigate: There's little doc on what can be done within this method. If its like an ISR
//        // then it should not take too long or do much I/O, but writing to Serial does appear to work ok
//        echo(mac, data, len);
//
//    });
//    esp_now_add_peer(MASTER_MAC, ESP_NOW_ROLE_SLAVE, WIFI_CHANNEL, nullptr, 0);  // Initialize as sender
//}
//
//
//void setup() {
//    // Set MAC
//    WiFi.mode(WIFI_AP);
//    wifi_set_macaddr(SOFTAP_IF, SELF_MAC);
//
//    Serial.begin(115200);
//    Serial.println();
//    Serial.print("This node AP mac: ");
//    Serial.println(WiFi.softAPmacAddress());
//    Serial.println("Initializing ESPNow ...");
//    initEspNow();
//    Serial.println("ESPNow Server Ready");
//}
//
//void loop() {
//    if (millis() - lastTime > 1000) {
//        Serial.print(".");
//        lastTime = millis();
//    }
//}