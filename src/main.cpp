#include <Arduino.h>
#include "Config.h" 
#include "EncoderDriver.h"
#include "LEDDriver.h"
#include "ColorUtils.h"


BtnDriver modeButton(Pins::BTN);
RotEncDriver rotaryEncoder(Pins::ENC_A, Pins::ENC_B);
LEDDriver ledDriver(Pins::RED, Pins::GREEN, Pins::BLUE);

int8_t rotation = 0;
bool short_pressed = false;
bool long_pressed = false;
uint8_t mode = 0;
// mode 0 == hue adjust
// mode 1 == saturation adjust
// mode 2 == brightness adjust

// Load color from EEPROM
HSV colorHSV = loadDefaultColor();
unsigned long last_rot_time = 0;

void setup(){ 

  modeButton.begin();
  rotaryEncoder.begin();
  ledDriver.begin();

  ledDriver.setHSV(colorHSV);
  if (Config::SERIAL_OUTPUT){
    Serial.begin(Config::SERIAL_BAUDRATE);
    Serial.println("Button instanciated - Setup Complete");
  }
}

void loop(){
  modeButton.update();
  rotaryEncoder.update();

  short_pressed = modeButton.get_press();
  long_pressed = modeButton.get_long_press();
  rotation = rotaryEncoder.get_rotation();

  // Handle Save Command
  if (long_pressed) {
    saveDefaultColor(colorHSV);
    
    if (Config::SERIAL_OUTPUT) Serial.println("color saved to EEPROM");
    
    // Visual feedback: Flash White
    ledDriver.setHSV({0, 0, 190}); 
    delay(200);
    ledDriver.setHSV({0, 0, 0}); 
    delay(200);
    ledDriver.setHSV({0, 0, 190}); 
    delay(200);
    ledDriver.setHSV({0, 0, 0}); 
    delay(200);
    ledDriver.setHSV(colorHSV);
  }

  // Update mode / HSV values
  if (short_pressed){
    mode = (mode + 1) % 3;
  }
  if (rotation != 0){
    last_rot_time = millis();
    switch (mode) {
      // hue mode
      case (0):{
        colorHSV.h = colorHSV.h + rotation * Config::HUE_STEP;
        break;
      }
      // saturation mode
      case (1):{
        int16_t new_s = (int16_t)colorHSV.s + (rotation * Config::SAT_STEP);
        colorHSV.s = constrain(new_s, 0, 255);
        break;
      }
      // brightness mode
      case (2):{
        int16_t new_v = (int16_t)colorHSV.v + (rotation * Config::BRIGHT_STEP);
        colorHSV.v = constrain(new_v, 25, 255);
        break;
      }
    }
    // Update LED
    ledDriver.setHSV(colorHSV);
  }

  // Output user inputs and colors to serial
  if (Config::SERIAL_OUTPUT){
    if (short_pressed){
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
