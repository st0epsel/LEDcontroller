#pragma once
#include <Arduino.h>

struct RGB {
    float r; // 0.0 to 1.0
    float g; // 0.0 to 1.0
    float b; // 0.0 to 1.0
};

struct HSV {
    float h; // 0.0 to 1.0 with rollover
    float s; // 0.0 to 1.0 no rollover
    float v; // 0.0 to 1.0 no rollover
};

void saveDefaultColor(HSV color);

HSV loadDefaultColor();

RGB hsv_to_rgb(HSV in);

void printRGB(const RGB& colorRGB);

void printHSV(const HSV& colorHSV);