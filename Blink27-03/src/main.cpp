#include <Arduino.h>
#include "ledfunctions.h"


uint8_t ledPinA = 0; //pin of led
uint8_t ledPinB = 14;
uint32_t delaytime_ms = 100; 
//uint32_t prevtime_ms = 0;
//uint32_t currenttime_ms = 0;
bool ledstate = LOW;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize External LED as an output.
  pinMode(ledPinA, OUTPUT);
  pinMode(ledPinB, OUTPUT);
 }

void loop() {
  static uint32_t prevtime_ms = 0; //static makes the value execute on the first call
  uint32_t currenttime_ms = millis();

  if ((currenttime_ms-prevtime_ms) >= delaytime_ms) {
    //reset timer
    prevtime_ms = currenttime_ms;
    toggleLED(ledPinA);
    toggleLED(ledPinB);
    
    
    //digitalWrite(ledPin, !digitalRead(ledPin));
  }
}

