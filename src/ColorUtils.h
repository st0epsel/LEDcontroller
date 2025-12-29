#pragma once
#include <Arduino.h>

struct RGB {
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

struct HSV {
    uint8_t h; // 0 to 255 with rollover
    uint8_t s; // 0 to 255 no rollover
    uint8_t v; // 0 to 255 no rollover
};

// Just the name and the "shape" of the function
RGB hsv_to_rgb(HSV in);