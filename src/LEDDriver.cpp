#include <Arduino.h>
#include "ColorUtils.h"
#include "LEDDriver.h"
#include "Config.h"

const byte gamma8[] PROGMEM = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,  1,  1,  1,  1,
    1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,  2,  3,  3,  3,  3,
    3,  3,  4,  4,  4,  4,  5,  5,  5,  5,  6,  6,  6,  6,  7,  7,
    7,  7,  8,  8,  8,  9,  9,  9, 10, 10, 10, 11, 11, 11, 12, 12,
    13, 13, 13, 14, 14, 15, 15, 16, 16, 17, 17, 18, 18, 19, 19, 20,
    20, 21, 21, 22, 22, 23, 24, 24, 25, 25, 26, 27, 27, 28, 29, 29,
    30, 31, 32, 32, 33, 34, 35, 35, 36, 37, 38, 39, 39, 40, 41, 42,
    43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 54, 55, 56, 57, 58, 59,
    60, 61, 62, 64, 65, 66, 67, 68, 70, 71, 72, 73, 75, 76, 77, 79,
    80, 81, 83, 84, 85, 87, 88, 90, 91, 92, 94, 95, 97, 98, 100, 101,
    103, 104, 106, 107, 109, 111, 112, 114, 115, 117, 119, 120, 122, 124, 126, 127,
    129, 131, 133, 135, 137, 138, 140, 142, 144, 146, 148, 150, 152, 154, 156, 158,
    160, 162, 164, 167, 169, 171, 173, 175, 177, 180, 182, 184, 186, 189, 191, 193,
    196, 198, 200, 203, 205, 208, 210, 213, 215, 218, 220, 223, 225, 228, 231, 233,
    236, 239, 241, 244, 247, 249, 252, 255 };


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
    RGB raw = hsv_to_rgb(in);

    if (Config::LOG_BRIGHTNESS) {
        // use pgm_read_byte because the table is stored in Flash (PROGMEM)
        Serial.print("Raw - R: "); Serial.print(raw.r);
        Serial.print(" G: "); Serial.print(raw.g);
        Serial.print(" B: "); Serial.println(raw.b);

        uint8_t correctedR = pgm_read_byte(&gamma8[raw.r]);
        uint8_t correctedG = pgm_read_byte(&gamma8[raw.g]);
        uint8_t correctedB = pgm_read_byte(&gamma8[raw.b]);

        Serial.print("gamma8 - R: "); Serial.print(correctedR);
        Serial.print(" G: "); Serial.print(correctedG);
        Serial.print(" B: "); Serial.println(correctedB);
        Serial.println();

        analogWrite(_pinR, correctedR);
        analogWrite(_pinG, correctedG);
        analogWrite(_pinB, correctedB);
    } else {
        analogWrite(_pinR, raw.r);
        analogWrite(_pinG, raw.g);
        analogWrite(_pinB, raw.b);
    }
}

void LEDDriver::setRGB(RGB colorRGB) {
    analogWrite(_pinR, colorRGB.r);
    analogWrite(_pinG, colorRGB.g);
    analogWrite(_pinB, colorRGB.b);
}