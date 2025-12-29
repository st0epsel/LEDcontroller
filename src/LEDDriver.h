#pragma once
#include <Arduino.h>
#include "ColorUtils.h"

class LEDDriver {
private:
    uint8_t _pinR;
    uint8_t _pinG;
    uint8_t _pinB;

public:
    LEDDriver(uint8_t pinR, uint8_t pinG, uint8_t pinB);

    void begin();

    void setHSV(HSV colorHSV);

    void setRGB(RGB colorRGB);
};