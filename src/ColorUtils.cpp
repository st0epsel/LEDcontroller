#include <Arduino.h>
#include <stdint.h>
#include <avr/pgmspace.h>
#include <EEPROM.h>
#include "Config.h"
#include "ColorUtils.h"


void saveDefaultColor(HSV color) {
    EEPROM.put(0, color); 
}

HSV loadDefaultColor() {
    HSV color;
    EEPROM.get(0, color); 
    return color;
}

RGB hsv_to_rgb(HSV in) {
    float r, g, b;

    // Normalize H, S, and V to usable float ranges
    // H is mapped to 0.0 - 6.0 (6 sectors)
    // S and V are mapped to 0.0 - 1.0
    float h = in.h * 6.0f;
    float s = in.s;
    float v = in.v;

    if (in.s == 0.0f) {
        // Achromatic (gray/white/black)
        r = g = b = v;
    } else {
        uint8_t i = (uint8_t)h;             // Sector index (0 to 5)
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
        constrain(r, 0.0f, 1.0f),
        constrain(g, 0.0f, 1.0f),
        constrain(b, 0.0f, 1.0f)
    };
}

void printRGB(const RGB& colorRGB) {
    Serial.print("R: "); Serial.print(colorRGB.r);
    Serial.print(" G: "); Serial.print(colorRGB.g);
    Serial.print(" B: "); Serial.println(colorRGB.b);
}

void printHSV(const HSV& colorHSV) {
    Serial.print("H: "); Serial.print(colorHSV.h);
    Serial.print(" S: "); Serial.print(colorHSV.s);
    Serial.print(" V: "); Serial.println(colorHSV.v);
}
