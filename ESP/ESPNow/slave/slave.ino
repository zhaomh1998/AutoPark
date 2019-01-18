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
#define SELF_ROLE 1  // Who am I in the system?
#define WIFI_CHANNEL 4

#define MASTER 0
#define CAR 1
#define ELEVATOR 2
#define FLOOR1 3
#define FLOOR2 4
uint8_t MASTER_MAC[] = {0x36, 0x00, 0x00, 0x00, 0x66, 0x33};
uint8_t CAR_MAC[] = {0x36, 0x00, 0x00, 0x00, 0x66, 0x34};
uint8_t ELEVATOR_MAC[] = {0x36, 0x00, 0x00, 0x00, 0x66, 0x35};
uint8_t FLOOR_1_MAC[] = {0x36, 0x00, 0x00, 0x00, 0x66, 0x36};
uint8_t FLOOR_2_MAC[] = {0x36, 0x00, 0x00, 0x00, 0x66, 0x37};
uint8_t* MACS[] = {MASTER_MAC, CAR_MAC, ELEVATOR_MAC, FLOOR_1_MAC, FLOOR_2_MAC, NULL};

/**
   @brief     Set up self MAC Address
*/
void initVariant() {
  WiFi.mode(WIFI_AP);
  wifi_set_macaddr(SOFTAP_IF, &MACS[SELF_ROLE][0]);
}

void setup() {
  Serial.begin(115200); Serial.println();

  Serial.print("This node AP mac: "); Serial.println(WiFi.softAPmacAddress());
  Serial.println("Initializing ESPNow ...");
  initEspNow();
  Serial.println("ESPNow Server Ready");
    uint8_t bs[] = "Hello ESP!";
    sendEspNow(MASTER_MAC, bs);
}

int lastTime;

void loop() {
  if (millis() - lastTime > 1000) {
    Serial.print(".");
    lastTime = millis();
  }
}

/**
   @brief     Send bytes through ESP Now to target mac address

   @param     mac  MAC Address to send message to
   @param     msg  Bytes array containing the data to send
*/

void sendEspNow(uint8_t *mac, uint8_t *msg) {
  Serial.printf("Sending to %02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  Serial.write(msg, sizeof(msg));
  esp_now_send(mac, msg, sizeof(msg)); // set mac parameter to NULL to send to all devices
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

    // Serial.printf("Message from %02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    // Serial.print(", Length: ");
    // Serial.print(len);
    // Serial.print("Message->");
    // Serial.write(data, len);
    // Serial.println("<-");
    // sendEspNow(mac, data); //Echo msg back
    msgHandle(mac, data, len);

  });
  for (int nthClient = 0; nthClient < sizeof(MACS) / sizeof(MACS[0]); nthClient++) {
    esp_now_add_peer(MACS[nthClient], ESP_NOW_ROLE_SLAVE, WIFI_CHANNEL, NULL, 0);  // Initialize as sender
  }
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

/**
   @brief     Handles message receive event for ESP_Now

   @param     msgSourceMac  Message source's MAC Addr in bytes array
   @param     msg  Content of the message in bytes array
   @param     msgLen  Length of the message

   @return    Status (true/false)
*/

bool msgHandle(uint8_t *msgSourceMac, uint8_t *msg, uint8_t msgLen) {
  int msgSource = searchMac(msgSourceMac, MACS);
  // Serial.printf("Message from %02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  Serial.print(msgSource);
  Serial.print("is message source, Length: ");
  Serial.print(msgLen);
  Serial.print("Message->");
  Serial.write(msg, msgLen);
  Serial.println("<-");
  sendEspNow(msgSourceMac, msg); //Echo msg back
}
