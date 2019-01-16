/**
   ESP-NOW Echo server

   This sketch is heavily based on
      Anthony Elder's work: https://github.com/HarringayMakerSpace/ESP-Now
      Andreas Spiess's work: https://github.com/SensorsIot/ESP-Now-Tests
*/
#include <ESP8266WiFi.h>
extern "C" {
#include <espnow.h>
#include "user_interface.h"
}

/* Set a private Mac Address
    http://serverfault.com/questions/40712/what-range-of-mac-addresses-can-i-safely-use-for-my-virtual-machines
   Note: the point of setting a specific MAC is so you can replace this Gateway ESP8266 device with a new one
   and the new gateway will still pick up the remote sensors which are still sending to the old MAC
*/
uint8_t myMAC[] = {0x36, 0x33, 0x33, 0x33, 0x33, 0x33};
uint8_t remoteMAC[] = {0x36, 0x33, 0x33, 0x33, 0x33, 0x34};
#define WIFI_CHANNEL 4

void initVariant() {
  WiFi.mode(WIFI_AP);
  wifi_set_macaddr(SOFTAP_IF, &myMAC[0]);
}

void setup() {
  Serial.begin(115200); Serial.println();

  Serial.print("This node AP mac: "); Serial.println(WiFi.softAPmacAddress());
  Serial.print("This node STA mac: "); Serial.println(WiFi.macAddress());
  Serial.println("Initializing ESPNow ...");
  initEspNow();
  Serial.println("ESPNow Server Ready");
}

int lastTime;

void loop() {
  if (millis() - lastTime > 1000) {
    Serial.print(".");
    lastTime = millis();
  }
}

void sendEspNow(uint8_t *mac, uint8_t *msg) {
  //  uint8_t bs[] = msg;
  Serial.printf("Sending to %02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  Serial.write(msg, sizeof(msg));
  esp_now_send(mac, msg, sizeof(msg)); // set mac parameter to NULL to send to all devices
}

void initEspNow() {
  if (esp_now_init() != 0) {
    Serial.println("*** ESP_Now init failed");
    ESP.restart();
  }

  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
  esp_now_add_peer(remoteMAC, ESP_NOW_ROLE_SLAVE, WIFI_CHANNEL, NULL, 0);  // Initialize as sender
  esp_now_register_recv_cb([](uint8_t *mac, uint8_t *data, uint8_t len) {  // Initialize as receiver

    // Investigate: There's little doc on what can be done within this method. If its like an ISR
    // then it should not take too long or do much I/O, but writing to Serial does appear to work ok

    Serial.printf("Message from %02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    Serial.print(", Length: ");
    Serial.print(len);
    Serial.print("Message->");
    Serial.write(data, len);
    Serial.println("<-");
    sendEspNow(mac, data); //Echo msg back

  });
}
