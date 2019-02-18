//
// Created by Scott Zhao on 2019-02-17.
//

#include "Arduino.h"

void setup() {
    Serial.begin(115200);
}

void loop() {
//    digitalWrite(BUILTIN_LED, LOW);
//    delay(1000);
//    digitalWrite(BUILTIN_LED, HIGH);
//    delay(2000);
    Serial.println("hello world!");
    delay(100);
}