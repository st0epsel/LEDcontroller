#include <Arduino.h>
#include "EncoderDriver.h"
#include "Config.h"

BtnDriver::BtnDriver(uint8_t pin) {
    _pin = pin;
    _last_state = HIGH;
    _last_debounce_time = 0;
}

void BtnDriver::begin() {
    pinMode(_pin, INPUT_PULLUP);
}

bool BtnDriver::get_press() {
    bool current_reading = digitalRead(_pin);
    bool pressed = false;

    if ((millis() - _last_debounce_time) > Config::btn_min_delay) {
        if (current_reading != _last_state) {
        //  only register if Config::btn_min_delay ms have passed since last change
        
            if (current_reading == LOW) {
                pressed = true;
            }
            _last_debounce_time = millis();
        }
        _last_state = current_reading;
    }

    return pressed;
}

RotEncDriver::RotEncDriver(uint8_t pinA, uint8_t pinB) {
    _pinA = pinA;
    _pinB = pinB;
    _last_state_A = HIGH;
    _last_state_B = HIGH;
    _last_debounce_time_A = 0;
    _last_debounce_time_B = 0;
}

void RotEncDriver::begin() {
    pinMode(_pinA, INPUT_PULLUP);
    pinMode(_pinB, INPUT_PULLUP);
}

int8_t RotEncDriver::get_rotation() {
    bool current_state_A = digitalRead(_pinA);
    bool current_state_B = digitalRead(_pinB);
    int8_t rotation = 0;

    // pin A
    if ((millis() - _last_debounce_time_A) > Config::rot_eng_min_delay) {
        if (current_state_A != _last_state_A) {
            if (current_state_A == LOW) {
                // Pin A is low, check pin B to determine direction
                if (digitalRead(_pinB) == HIGH) {
                    rotation = 1; // Clockwise
                } else {
                    rotation = -1; // Counter-clockwise
                }
            }
            _last_debounce_time_A = millis();
        }
        _last_state_A = current_state_A;
    }

    // pin B
    if ((millis() - _last_debounce_time_B) > Config::rot_eng_min_delay) {
        if (current_state_B != _last_state_B) {
            if (current_state_B == LOW) {
                // Pin B is low, check pin A to determine direction
                if (digitalRead(_pinA) == HIGH) {
                    rotation = -1; // Counter-clockwise
                } else {
                    rotation = 1; // Clockwise
                }
            }
            _last_debounce_time_B = millis();
        }
        _last_state_B = current_state_B;
    }

    return rotation;
}