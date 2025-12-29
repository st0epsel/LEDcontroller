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

    constexpr HSV DEFAULT_COLOR = {180, 255, 127};

    constexpr uint8_t btn_min_delay = 200;
    constexpr uint8_t rot_eng_min_delay = 80;
    constexpr uint16_t long_press_duration_ms = 1500;

    // --- SERIAL OUTPUT ---
    constexpr bool SERIAL_OUTPUT = false;
    constexpr uint32_t SERIAL_BAUDRATE = 115200;
}