#pragma once
#include <Arduino.h>

class BtnDriver {
private:
    uint8_t _pin;           // The hardware pin number
    bool _last_state;       // To track if the button was high or low last time
    unsigned long _last_debounce_time; // To prevent "flicker" (bounce)

public:
    // Constructor: Tells the class which pin to watch
    BtnDriver(uint8_t pin);

    // Initializes the pin mode
    void begin();

    // The function you requested to check for a new press
    bool get_press();

    // Get a long press
    bool get_long_press();
};

class RotEncDriver {
private:
    uint8_t _pinA;
    uint8_t _pinB;
    bool _last_state_A;
    bool _last_state_B;  
    unsigned long _last_debounce_time_A; 
    unsigned long _last_debounce_time_B;

public:
    // Constructor: Tells the class which pin to watch
    RotEncDriver(uint8_t pinA, uint8_t pinB);

    // Initializes the pin mode
    void begin();

    // The function you requested to check for a new press
    int8_t get_rotation();
};