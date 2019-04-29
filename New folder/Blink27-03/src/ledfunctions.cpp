#include <Arduino.h>

void toggleLED(uint8_t ledPin){
  uint8_t ledstate = digitalRead(ledPin);
  if(ledstate == HIGH){//if LED is ON, Switch LED off
    ledstate = LOW;
    } 
    else{ //Switch led ON
    ledstate = HIGH;
    }
    digitalWrite(ledPin, ledstate);
}