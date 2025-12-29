#pragma once
#include <Arduino.h>

struct RGB { uint8_t r; uint8_t g; uint8_t b; };
struct HSV { uint8_t h; uint8_t s; uint8_t v; };

// Just the name and the "shape" of the function
RGB hsv_to_rgb(HSV in);