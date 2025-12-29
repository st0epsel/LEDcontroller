#include <Arduino.h>
#include "EncoderDriver.h"
#include "Config.h" 
#include "ColorUtils.h"
#include "LEDDriver.h"

BtnDriver modeButton(Pins::BTN);
RotEncDriver rotaryEncoder(Pins::ENC_A, Pins::ENC_B);
LEDDriver ledDriver(Pins::RED, Pins::GREEN, Pins::BLUE);

int8_t rotation = 0;
bool buttonPressed = false;
uint8_t mode = 0;
// mode 0 == hue adjust
// mode 1 == saturation adjust
// mode 2 == brightness adjust
HSV colorHSV = Config::DEFAULT_COLOR;

void setup(){ 
  Serial.begin(Config::SERIAL_BAUDRATE);
  modeButton.begin();
  rotaryEncoder.begin();
  ledDriver.begin();
  ledDriver.setHSV(colorHSV);
  if (Config::SERIAL_OUTPUT){
    Serial.println("Button instanciated - Setup Complete");
  }
}

void loop(){
  buttonPressed = modeButton.get_press();
  rotation = rotaryEncoder.get_rotation();
  // Update mode / color
  if (buttonPressed){
    mode = (mode + 1) % 3;
  }
  if (rotation != 0){
    switch (mode) {
      case (0):{
        colorHSV.h = colorHSV.h + rotation * Config::HUE_STEP;
        break;
      }
      case (1):{
        int16_t new_s = (int16_t)colorHSV.s + (rotation * Config::SAT_STEP);
        colorHSV.s = new_s < 0 ? 0 : (new_s > 255 ? 255 : new_s);
        break;
      }
      case (2):{
        int16_t new_v = (int16_t)colorHSV.v + (rotation * Config::BRIGHT_STEP);
        colorHSV.v = new_v < 0 ? 0 : (new_v > 255 ? 255 : new_v);
        break;
      }
    }
    // Update LED
    ledDriver.setHSV(colorHSV);
  }
  // Output user inputs
  if (Config::SERIAL_OUTPUT){
    if (buttonPressed){
      Serial.print("BTN - Mode: "); Serial.println(mode);
    }
    if (rotation != 0){
      if (rotation > 0){
        Serial.println("CW");
      } else {
        Serial.println("CCW");
      }
      Serial.print("H: "); Serial.print(colorHSV.h);
      Serial.print(" S: "); Serial.print(colorHSV.s);
      Serial.print(" V: "); Serial.println(colorHSV.v);
    }
  }
}
