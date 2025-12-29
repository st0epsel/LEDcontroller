#pragma once
#include <Arduino.h>
#include "ColorUtils.h"

namespace Pins {
    // LED control pins
    constexpr uint8_t RED   = 3;
    constexpr uint8_t GREEN = 5;
    constexpr uint8_t BLUE  = 6;

    // rotary encoder & button pins
    constexpr uint8_t ENC_A = 9; // Best if these are Interrupt pins (2/3/4)
    constexpr uint8_t ENC_B = 8;
    constexpr uint8_t BTN   = 7;
}

// --- CALIBRATION ---
namespace Config {

    constexpr bool LOG_BRIGHTNESS = true;
    constexpr uint8_t HUE_STEP = 4; 
    constexpr uint8_t SAT_STEP = 8; 
    constexpr uint8_t BRIGHT_STEP = 8; 

    // --- DEBOUNCE & TIMINGS ---
    constexpr uint8_t BTN_DEBOUNCE_T = 50;
    constexpr uint16_t LONG_PRESS_DURATION = 1500;
    constexpr uint8_t ROT_ENC_DEBOUNCE_T = 30;

    // --- SERIAL OUTPUT ---
    constexpr bool SERIAL_OUTPUT = true;
    constexpr uint32_t SERIAL_BAUDRATE = 115200;

    // --- SOLDERING ---
    constexpr bool USE_INTERNAL_PULLUP = false;
}