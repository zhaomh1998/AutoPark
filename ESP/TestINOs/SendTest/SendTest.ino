/**
   ESP8266 ESP-NOW Echo server

   This sketch is heavily based on
      Anthony Elder's work: https://github.com/HarringayMakerSpace/ESP-Now
      Andreas Spiess's work: https://github.com/SensorsIot/ESP-Now-Tests
*/
#include <ESP8266WiFi.h>
extern "C" {
#include <espnow.h>
#include "user_interface.h"
}

// Define remote Mac Addr
/* Set a private Mac Address
    http://serverfault.com/questions/40712/what-range-of-mac-addresses-can-i-safely-use-for-my-virtual-machines
   Note: the point of setting a specific MAC is so you can replace this Gateway ESP8266 device with a new one
   and the new gateway will still pick up the remote sensors which are still sending to the old MAC
*/
#define WIFI_CHANNEL 4

uint8_t TARGET_MAC[] = {0x36, 0x00, 0x00, 0x00, 0x66, 0x33};
uint8_t MY_MAC[] = {0x36, 0x00, 0x00, 0x00, 0x66, 0x34};

/**
   @brief     Set up self MAC Address
*/
void initVariant() {
  WiFi.mode(WIFI_AP);
  wifi_set_macaddr(SOFTAP_IF, MY_MAC);
}

void setup() {
  Serial.begin(115200); Serial.println();

  Serial.print("This node AP mac: "); Serial.println(WiFi.softAPmacAddress());
  Serial.println("Initializing ESPNow ...");
  initEspNow();
  Serial.println("ESPNow Server Ready");
}

int lastTime;

void loop() {
  static uint8_t f[] = {0x00};
  static uint8_t b[] = {0x01};
  static uint8_t l[] = {0x02};
  static uint8_t r[] = {0x03};
  static uint8_t s[] = {0x04};
  sendEspNow(TARGET_MAC, f, 1);
  sendEspNow(TARGET_MAC, b, 1);
  sendEspNow(TARGET_MAC, l, 1);
  sendEspNow(TARGET_MAC, r, 1);
  sendEspNow(TARGET_MAC, s, 1);
  delay(1000);

}

/**
   @brief     Send bytes through ESP Now to target mac address

   @param     mac  MAC Address to send message to
   @param     msg  Bytes array containing the data to send
*/

void sendEspNow(uint8_t *mac, uint8_t *msg, int len) {
  Serial.printf("Sending to %02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  Serial.write(msg, sizeof(msg));
  Serial.println();
  esp_now_send(mac, msg, len); // set mac parameter to NULL to send to all devices
}

/**
   @brief     Initialize ESP Now as both the master and slave
*/
void initEspNow() {
  if (esp_now_init() != 0) {
    Serial.println("*** ESP_Now init failed");
    ESP.restart();
  }

  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
  esp_now_register_recv_cb([](uint8_t *mac, uint8_t *data, uint8_t len) {  // Initialize as receiver

    // Investigate: There's little doc on what can be done within this method. If its like an ISR
    // then it should not take too long or do much I/O, but writing to Serial does appear to work ok
    Serial.printf("Message from %02x:%02x:%02x:%02x:%02x:%02x||len=%d, msg->", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5], len);
    for (int i = 0; i < len; i++) {
      Serial.printf("%x", data[i]);
    }
    Serial.println();
    });
    esp_now_add_peer(TARGET_MAC, ESP_NOW_ROLE_SLAVE, WIFI_CHANNEL, NULL, 0);
  }

  /**
     @brief     Search a MAC address in a list of them, and return its index if found

     @param     inMac  MAC Address to be searched
     @param     macList List of MAC Addresses to be searched from

     @return
              - -1 : MAC not found
              - nonnegative integer : index of the inMac in macList
  */

  int searchMac(uint8_t *inMac, uint8_t **macList) {
    for (int nthClient = 0; macList[nthClient] != NULL; nthClient++) {
      for (int nthByte = 0; nthByte < 6; nthByte++) { // An MAC Addr contains 6 bytes
        if (inMac[nthByte] != macList[nthClient][nthByte])
          break;
        else if (nthByte == 5) // It passed tests all the way to the last byte, and last byte is the same (didn't break
          return nthClient;
      }
    }
    return -1; // Not found
  }
