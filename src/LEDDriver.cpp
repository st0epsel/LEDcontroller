#include <Arduino.h>
#include "ColorUtils.h"
#include "LEDDriver.h"

const byte gamma8[] PROGMEM = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,
    1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,  2,  3,  3,  3,  3,
    3,  3,  4,  4,  4,  4,  5,  5,  5,  5,  5,  6,  6,  6,  6,  7,
    7,  7,  8,  8,  8,  9,  9,  9, 10, 10, 10, 11, 11, 11, 12, 12,
    13, 13, 13, 14, 14, 15, 15, 16, 16, 17, 17, 18, 18, 19, 19, 20,
    20, 21, 21, 22, 22, 23, 24, 24, 25, 25, 26, 27, 27, 28, 29, 29,
    30, 31, 31, 32, 33, 34, 34, 35, 36, 37, 38, 38, 39, 40, 41, 42,
    43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58,
    59, 60, 61, 62, 63, 64, 65, 67, 68, 69, 70, 71, 72, 74, 75, 76,
    77, 79, 80, 81, 83, 84, 85, 87, 88, 89, 91, 92, 94, 95, 97, 98,
    100, 101, 103, 104, 106, 107, 109, 110, 112, 114, 115, 117, 119, 120, 122, 124,
    125, 127, 129, 130, 132, 134, 136, 137, 139, 141, 143, 145, 147, 149, 151, 153,
    155, 157, 159, 161, 163, 165, 167, 169, 171, 173, 175, 177, 179, 181, 184, 186,
    188, 190, 192, 195, 197, 199, 201, 204, 206, 208, 211, 213, 215, 218, 220, 223,
    225, 227, 230, 232, 235, 237, 240, 242, 245, 247, 250, 252, 255
};


LEDDriver::LEDDriver(uint8_t pinR, uint8_t pinG, uint8_t pinB){
    _pinR = pinR;
    _pinG = pinG;
    _pinB = pinB;
}

void LEDDriver::begin() {
    pinMode(_pinR, OUTPUT);
    pinMode(_pinG, OUTPUT);
    pinMode(_pinB, OUTPUT);
}

void LEDDriver::setHSV(HSV in) {
    // 1. Convert the HSV values to raw RGB (0-255)
    // This uses the function we just updated
    RGB raw = hsv_to_rgb(in);

    // 2. Apply Gamma Correction to each channel
    // We use pgm_read_byte because the table is stored in Flash (PROGMEM)
    uint8_t correctedR = pgm_read_byte(&gamma8[raw.r]);
    uint8_t correctedG = pgm_read_byte(&gamma8[raw.g]);
    uint8_t correctedB = pgm_read_byte(&gamma8[raw.b]);

    // 3. Write to the hardware pins
    analogWrite(_pinR, correctedR);
    analogWrite(_pinG, correctedG);
    analogWrite(_pinB, correctedB);
}

void LEDDriver::setRGB(RGB colorRGB) {
    analogWrite(_pinR, colorRGB.r);
    analogWrite(_pinG, colorRGB.g);
    analogWrite(_pinB, colorRGB.b);
}