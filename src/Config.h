#pragma once
#include <Arduino.h>
#include "ColorUtils.h"

namespace Pins {
    // LED control pins
    constexpr uint8_t RED   = 9;
    constexpr uint8_t GREEN = 10;
    constexpr uint8_t BLUE  = 11;

    // rotary encoder & button pins
    constexpr uint8_t ENC_A = 7; // Best if these are Interrupt pins (2/3/4)
    constexpr uint8_t ENC_B = 6;
    constexpr uint8_t BTN   = 4;
}

// --- CALIBRATION ---
namespace Config {
    constexpr uint8_t HUE_STEP = 8; 
    constexpr uint8_t SAT_STEP = 8; 
    constexpr uint8_t BRIGHT_STEP = 8; 

    constexpr HSV DEFAULT_COLOR = {90, 127, 127};

    constexpr uint8_t btn_min_delay = 200;
    constexpr uint8_t rot_eng_min_delay = 50;

    constexpr bool SERIAL_OUTPUT = true;
    constexpr uint32_t SERIAL_BAUDRATE = 115200;
}