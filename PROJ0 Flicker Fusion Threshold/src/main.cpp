#include <Arduino.h>

uint32_t ledlight = 0;
int ledState = LOW;
unsigned long previousMillis = 0;
float interval;
float value = 0;
float frequency = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(ledlight, OUTPUT);
  Serial.begin(9600);
  
}

void loop() {
   unsigned long currentMillis = millis();
   value = analogRead(A0);
   frequency = (map(value, 0 , 1024, 0 , 100));
   interval = (1/frequency) * 1000;

  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }

    // set the LED with the ledState of the variable:
    digitalWrite(ledlight, ledState);
    Serial.print(analogRead(A0));
    Serial.print(", ");
  }
}