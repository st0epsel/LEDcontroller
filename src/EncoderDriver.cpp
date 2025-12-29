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
    // 1. Read the current pins and combine them into a 2-bit number
    uint8_t current_state = (digitalRead(_pinA) << 1) | digitalRead(_pinB);
    static uint8_t encoder_history = 0;
    int8_t result = 0;

    // 2. If the state hasn't changed, do nothing
    if (current_state == (encoder_history & 0x03)) {
        return 0;
    }

    // 3. Store the new state in the history (keeping the last 4 bits)
    // History looks like: [Old_A][Old_B][New_A][New_B]
    encoder_history = ((encoder_history << 2) | current_state) & 0x0F;
    
    // Clockwise transitions
    if (encoder_history == 0b0111) result = -1; 
    
    // Counter-clockwise transitions
    if (encoder_history == 0b1011) result = 1;

    return result;
}