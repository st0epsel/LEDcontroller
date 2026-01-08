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

    // --- BRIGHTNESS CONTROL ---
    constexpr bool LOG_BRIGHTNESS = true;
    constexpr float HUE_STEP = 0.015f; 
    constexpr float SAT_STEP = 0.03f; 
    constexpr float BRIGHT_STEP = 0.03f; 
    constexpr float LED_FADE_MIN = 0.005f;
    constexpr float BRIGHTNESS_GAIN = 1.0f; // color intensity change per second

    constexpr HSV FALLBACK_COLOR = {0.0f, 0.0f, 1.0f};

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