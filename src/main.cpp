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
HSV DEFAULT_COLOR = {0.0f, 0.0f, 1.0f};
// mode 0 == hue adjust
// mode 1 == saturation adjust
// mode 2 == brightness adjust

unsigned long last_rot_time = 0;
HSV colorHSV;

void setup(){ 

  modeButton.begin();
  rotaryEncoder.begin();
  ledDriver.begin();

  // Load saved color from EEPROM
  colorHSV = loadDefaultColor();
  if (isnan(colorHSV.h) || isnan(colorHSV.s) || isnan(colorHSV.v)){
    colorHSV = Config::FALLBACK_COLOR; // Fallback to default if EEPROM is uninitialized or corrupted
  }
  ledDriver.set_HSV(colorHSV);

  if (Config::SERIAL_OUTPUT){
    Serial.begin(Config::SERIAL_BAUDRATE);
    Serial.println("... serial initialized");
    Serial.println("setup complete");
  }
}

void loop(){
  modeButton.update();
  rotaryEncoder.update();
  ledDriver.update();

  short_pressed = modeButton.get_press();
  long_pressed = modeButton.get_long_press();
  rotation = rotaryEncoder.get_rotation();

  // Handle Save Command
  if (long_pressed) {
    saveDefaultColor(colorHSV);
    
    if (Config::SERIAL_OUTPUT) Serial.println("color saved to EEPROM");
    
    // Visual feedback: Flash White
    ledDriver.write_RGB({0.8f, 0.8f, 0.8f}); 
    delay(200);
    ledDriver.write_RGB({0.0f, 0.0f, 0.0f}); 
    delay(200);
    ledDriver.write_RGB({0.8f, 0.8f, 0.8f}); 
    delay(200);
    ledDriver.write_RGB({0.0f, 0.0f, 0.0f}); 
    delay(200);
    ledDriver.set_HSV(colorHSV); // Restore previous color
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
        if (colorHSV.h < 0.0f) colorHSV.h += 1.0f;
        if (colorHSV.h > 1.0f) colorHSV.h -= 1.0f;
        break;
      }
      // saturation mode
      case (1):{
        colorHSV.s = constrain(colorHSV.s + rotation * Config::SAT_STEP, 0.0f, 1.0f);
        break;
      }
      // brightness mode
      case (2):{
        colorHSV.v = constrain(colorHSV.v + rotation * Config::BRIGHT_STEP, 0.1f, 1.0f);
        break;
      }
    }
    // Update LED
    ledDriver.set_HSV(colorHSV); // updates target color (only get updated in next loop cycle)
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
      printHSV(colorHSV);
    }
  }
}
