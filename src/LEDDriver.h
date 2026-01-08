#pragma once
#include <Arduino.h>
#include "ColorUtils.h"

class LEDDriver {
private:
    uint8_t _pinR;
    uint8_t _pinG;
    uint8_t _pinB;

    HSV _target_HSV;
    HSV _current_HSV;
    bool _target_color_reached;
    unsigned long _last_time;

public:
    LEDDriver(uint8_t pinR, uint8_t pinG, uint8_t pinB);

    void begin();

    void set_HSV(HSV colorHSV);

    void set_RGB(RGB colorRGB);

    void update();

    void write_RGB(RGB colorRGB);

    void gamma_correction(RGB& colorRGB);

};