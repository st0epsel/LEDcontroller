#pragma once
#include <Arduino.h>

class BtnDriver {
public:
    BtnDriver(uint8_t pin);
    void begin();
    void update(); 
    bool get_press();
    bool get_long_press();

private:
    uint8_t _pin;
    
    // State Variables
    bool _current_reading;
    bool _past_reading;
    unsigned long _last_change_time; // Time of the last accepted state change
    
    // Logic Variables
    bool _long_press_completed;
    
    // Output Flags
    bool _short_press_pending;
    bool _long_press_pending;
};

class RotEncDriver {
public:
    RotEncDriver(uint8_t pinA, uint8_t pinB);
    void begin();
    
    // Core state machine update
    void update(); 
    
    // Returns the total rotation since the last check and resets to 0
    int8_t get_rotation();

private:
    uint8_t _pinA;
    uint8_t _pinB;
    uint8_t _history;      // Stores [OldA][OldB][NewA][NewB]
    int8_t _accumulator;   // Sums up steps between loop cycles
};