#include <Arduino.h>
#include "lib/Communication/master.h"


// Roles: MASTER, FLOOR1,2,3, CAR1,2,3
const int MY_ROLE = MASTER;
master now(macs[MY_ROLE]);

void setup() {
    now.addAll(macs, MY_ROLE);
}

void loop() {
    now.send(macs[CAR1], macs[MY_ROLE], 6);
    delay(500);
}