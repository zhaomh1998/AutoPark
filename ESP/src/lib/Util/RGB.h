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
        processingToggle = true;
    }

    void ready() {
        setColor(0, 35, 0);
    }

    void processing() {
        if (processingToggle)
            setColor(25, 10, 0);
        else
            setColor(50, 20, 0);
        processingToggle = !processingToggle;
    }

    void warning() {
        setColor(35, 10, 0);
    }

    void error() {
        setColor(35, 0, 0);
    }

    void setColor(uint8_t r, uint8_t g, uint8_t b) {
        led.setPixelColor(0, r, g, b);
        led.show();

    }

private:
    Adafruit_NeoPixel led;
    bool processingToggle;
};


#endif //ESP_RGB_H
