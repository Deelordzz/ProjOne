#include <Arduino.h>

uint8_t ledPin = 0; //pin of led
uint32_t delaytime_ms = 50; 
//uint32_t prevtime_ms = 0;
//uint32_t currenttime_ms = 0;
bool ledstate = LOW;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize External LED as an output.
  pinMode(ledPin, OUTPUT);
 }

void loop() {
  static uint32_t prevtime_ms = 0; //static makes the value execute on the first call
  uint32_t currenttime_ms = millis();

  if ((currenttime_ms-prevtime_ms) >= delaytime_ms) {
    //reset timer
    prevtime_ms = currenttime_ms;
    //toggle the led
    if(ledstate == HIGH){//if LED is ON, Switch LED off
    ledstate = LOW;
    } 
    else{ //Switch led ON
    ledstate = HIGH;
    }
    digitalWrite(ledPin, ledstate);
    //digitalWrite(ledPin, !digitalRead(ledPin));
  }
}   