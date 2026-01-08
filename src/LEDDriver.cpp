#include <Arduino.h>
#include "ColorUtils.h"
#include "LEDDriver.h"
#include "Config.h"


template <typename T>
inline T cube(T x) {
    return x * x * x;
}


LEDDriver::LEDDriver(uint8_t pinR, uint8_t pinG, uint8_t pinB){
    _pinR = pinR;
    _pinG = pinG;
    _pinB = pinB;
}

void LEDDriver::begin() {
    pinMode(_pinR, OUTPUT);
    pinMode(_pinG, OUTPUT);
    pinMode(_pinB, OUTPUT);
    _target_HSV = {0.0f, 0.0f, 0.0f};
    _current_HSV = {0.0f, 0.0f, 0.0f};
    _target_color_reached = false;
    _last_time = millis();
}

void LEDDriver::gamma_correction(RGB& colorRGB) {
    // Use approximated gamme = 3 instead of more accurate but computationally expensive gamma = 2.8
    colorRGB.r = cube(colorRGB.r);
    colorRGB.g = cube(colorRGB.g);
    colorRGB.b = cube(colorRGB.b);
}

void LEDDriver::set_HSV(HSV in) {
    _target_HSV = in;
    _target_color_reached = false;
}

void LEDDriver::update() {
    unsigned long now = millis();
    unsigned long dt_ms = now - _last_time;
    _last_time = now;

    if (_target_color_reached) return;

    // Determine channel errors 
    // Hue channel needs wrapping due to circular nature
    float err_h = _target_HSV.h - _current_HSV.h;
    if (err_h > 0.5f) err_h -= 1.0f;       
    else if (err_h < -0.5f) err_h += 1.0f; 

    float err_s = _target_HSV.s - _current_HSV.s;
    float err_v = _target_HSV.v - _current_HSV.v;

    float max_err = max(max(abs(err_h), abs(err_s)), abs(err_v));
    if (max_err < Config::LED_FADE_MIN) {
        _current_HSV = _target_HSV;
        _target_color_reached = true;
    } else {
        float step = Config::BRIGHTNESS_GAIN * (dt_ms / 1000.0f);

        // Update each channel towards target
        if (abs(err_h) < step) _current_HSV.h = _target_HSV.h;
        else _current_HSV.h += (err_h > 0 ? step : -step);
        if (_current_HSV.h < 0.0f) _current_HSV.h += 1.0f;
        if (_current_HSV.h >= 1.0f) _current_HSV.h -= 1.0f;

        if (abs(err_s) < step) _current_HSV.s = _target_HSV.s;
        else _current_HSV.s += (err_s > 0 ? step : -step);

        if (abs(err_v) < step) _current_HSV.v = _target_HSV.v;
        else _current_HSV.v += (err_v > 0 ? step : -step);
    }
    write_RGB(hsv_to_rgb(_current_HSV));
}

void LEDDriver::write_RGB(RGB colorRGB) {
    if (Config::LOG_BRIGHTNESS) gamma_correction(colorRGB);
    printRGB(colorRGB);
    analogWrite(_pinR, constrain((uint8_t)(colorRGB.r*255), 0, 255));
    analogWrite(_pinG, constrain((uint8_t)(colorRGB.g*255), 0, 255));
    analogWrite(_pinB, constrain((uint8_t)(colorRGB.b*255), 0, 255));
}

