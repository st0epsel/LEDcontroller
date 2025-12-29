#include <Arduino.h>
#include <stdint.h>
#include <avr/pgmspace.h>
#include "Config.h"

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

RGB hsv_to_rgb(HSV in) {
    float r = 0, g = 0, b = 0;

    // 1. Normalize H, S, and V to usable float ranges
    // H is mapped to 0.0 - 6.0 (6 sectors)
    // S and V are mapped to 0.0 - 1.0
    float h = (in.h * 6.0f) / 255.0f;
    float s = in.s / 255.0f;
    float v = in.v / 255.0f;

    if (in.s == 0) {
        // Achromatic (gray/white/black)
        r = g = b = v;
    } else {
        int i = (int)h;             // Sector index (0 to 5)
        float f = h - i;            // Fractional part of H
        float p = v * (1.0f - s);
        float q = v * (1.0f - s * f);
        float t = v * (1.0f - s * (1.0f - f));

        switch (i) {
            case 0:  r = v; g = t; b = p; break;
            case 1:  r = q; g = v; b = p; break;
            case 2:  r = p; g = v; b = t; break;
            case 3:  r = p; g = q; b = v; break;
            case 4:  r = t; g = p; b = v; break;
            default: r = v; g = p; b = q; break; // Covers i=5 and rounding edge cases
        }
    }

    return {
        (uint8_t)(r * 255.0f),
        (uint8_t)(g * 255.0f),
        (uint8_t)(b * 255.0f)
    };
}
