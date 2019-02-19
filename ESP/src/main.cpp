#include <Arduino.h>
#include "lib/ESPNow.h"


uint8_t SELF_MAC[] = {0x36, 0x00, 0x00, 0x00, 0x66, 0x33};
uint8_t PEER_MAC[] = {0x36, 0x00, 0x00, 0x00, 0x66, 0x34};

ESPNow now(SELF_MAC);

void setup() {
    now.add_peer(PEER_MAC);
}

void loop() {
    now.send(PEER_MAC, SELF_MAC, 6);
    delay(500);
}