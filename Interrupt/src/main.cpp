#include <Arduino.h>
uint8_t count = 0;
uint8_t buttonPin = 12;
bool buttonPressed = 0;
bool buttonReleased = 0;

const uint32_t DEBOUNCETIME_MS = 50;

void something();
void ISR_button();

void setup() {
  // your setup code here, to run once:
Serial.begin(500000);
pinMode(buttonPin, INPUT_PULLUP);
attachInterrupt(digitalPinToInterrupt(buttonPin), ISR_button, FALLING); //FALLING, RISING, CHANGE
}

void loop() {
  // put your main code here, to run repeatedly:
/* 
if (buttonPressed ){
  buttonPressed = 0;
  count = count + 1; //count++;*/
 { Serial.print("count = ");
  Serial.println(count);
 }
 //buttonReleased = buttonPressed;


 something();

}
void something(){
  for (uint32_t i = 0; i <= 0xffff; i++) {
    Serial.print("*"); 
    Serial.println(count); 
  }
}

void ISR_button(){
  static uint32_t previousTime_ms = 0;
  uint32_t currentTime_ms = millis();

  if((currentTime_ms - previousTime_ms) > DEBOUNCETIME_MS){
    count++;
  }
  previousTime_ms = currentTime_ms;
}