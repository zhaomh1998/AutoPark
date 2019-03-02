#include "WS2812FX.h"

#define LED_COUNT 1
#define LED_PIN 15

WS2812FX ws2812fx = WS2812FX(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
    ws2812fx.init();
    ws2812fx.setBrightness(100);
    ws2812fx.setSpeed(200);
    ws2812fx.setMode(FX_MODE_RAINBOW);
    ws2812fx.start();
}

void loop() {
    ws2812fx.service();
}
