//
// Created by Scott Zhao on 2019-02-28.
//

#ifndef ESP_RGB_H
#define ESP_RGB_H

#include <Adafruit_NeoPixel.h>

class RGB {
public:
    RGB(uint8_t ledPin) {
        led = Adafruit_NeoPixel(1, ledPin, NEO_GRB + NEO_KHZ800);
        led.begin();
    }

    void ready() {
        setColor(0, 35, 0);
    }

    void processing() {
        setColor(25, 10, 0);
    }

    void warning() {
        setColor(35, 10, 0);
    }

    void error() {
        setColor(35, 0, 0);
    }

    void setColor(uint16_t r, uint16_t g, uint16_t b) {
        led.setPixelColor(0, r, g, b);
        led.show();
    }

private:
    Adafruit_NeoPixel led;
};


#endif //ESP_RGB_H