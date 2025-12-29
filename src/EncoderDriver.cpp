#include <Arduino.h>
#include "EncoderDriver.h"
#include "Config.h"

BtnDriver::BtnDriver(uint8_t pin) {
    _pin = pin;
    _current_reading = LOW;
    _past_reading = LOW;
    _last_change_time = 0;
    _short_press_pending = false;
    _long_press_pending = false;
    _long_press_completed = true;
}

void BtnDriver::begin() {
    if (Config::USE_INTERNAL_PULLUP) {
        pinMode(_pin, INPUT_PULLUP);
    } else {
        pinMode(_pin, INPUT);
    }
}

void BtnDriver::update() {
    // Debounce prevention: Ignore changes within the debounce time
    if ((millis() - _last_change_time) < Config::BTN_DEBOUNCE_T) {
        return; 
    }
    _past_reading = _current_reading;
    _current_reading = digitalRead(_pin);
    
    // Detect bare change
    if (_current_reading != _past_reading) {
        _last_change_time = millis();

        // Detect release
        if (_current_reading == LOW) {
            if (_long_press_completed && (millis() - _last_change_time <= Config::LONG_PRESS_DURATION)) {
                // Valid short press detected
                _short_press_pending = true;
            }
        }
    }      

    // Detect long press
    if (_long_press_completed) {
        if (_current_reading == HIGH && (millis() - _last_change_time >= Config::LONG_PRESS_DURATION)) {
            _long_press_pending = true;
            _long_press_completed = false; // Prevent multiple long press detections
            _current_reading = LOW;
        }
    } else if (_current_reading == LOW) {
        _long_press_completed = true; // Resolve long press state on button release
    }
}

bool BtnDriver::get_press() {
    if (_short_press_pending) {
        _short_press_pending = false;
        return true;
    }
    return false;
}

bool BtnDriver::get_long_press() {
    if (_long_press_pending) {
        _long_press_pending = false;
        return true;
    }
    return false;
}

RotEncDriver::RotEncDriver(uint8_t pinA, uint8_t pinB) {
    _pinA = pinA;
    _pinB = pinB;
    _history = 0;
    _accumulator = 0;
}

void RotEncDriver::begin() {
    if (Config::USE_INTERNAL_PULLUP) {
        pinMode(_pinA, INPUT_PULLUP);
        pinMode(_pinB, INPUT_PULLUP);
    } else {
        pinMode(_pinA, INPUT);
        pinMode(_pinB, INPUT);
    }
    
    uint8_t start_state = (digitalRead(_pinA) << 1) | digitalRead(_pinB);
    _history = start_state;
}

void RotEncDriver::update() {
    // 1. Read current pins and pack into 2 bits
    uint8_t current_state = (digitalRead(_pinA) << 1) | digitalRead(_pinB);
    
    // 2. Only act if the state has changed
    if (current_state != (_history & 0x03)) {
        
        // 3. Update history: Shift left 2 bits and add new state
        // Result: [OldA][OldB][NewA][NewB] (4 bits total)
        _history = ((_history << 2) | current_state) & 0x0F;

        // 4. Look for valid transitions into the "Detent" (Resting) state
        // Most common resting state is 11 (Both pins HIGH)
        
        // Clockwise: 01 -> 11 (History: 0b0111)
        if (_history == 0b0111) {
            _accumulator++;
        }
        // Counter-Clockwise: 10 -> 11 (History: 0b1011)
        else if (_history == 0b1011) {
            _accumulator--;
        }
    }
}

int8_t RotEncDriver::get_rotation() {
    // Return the accumulated value and clear it
    int8_t result = _accumulator;
    _accumulator = 0; 
    return result;
}