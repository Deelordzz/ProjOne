#include <Arduino.h>
uint32_t count = 0;
uint8_t buttonPin = 12;
bool buttonPressed = 0;
bool buttonReleased = 0;

void something();

void setup() {
  // your setup code here, to run once:
Serial.begin(500000);
pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  // put your main code here, to run repeatedly:
  buttonPressed = !digitalRead(buttonPin);
if (buttonPressed && (buttonReleased == 0)){
  count = count + 1; //count++;
  Serial.print("count = ");
  Serial.println(count);
 }
 buttonReleased = buttonPressed;


 something();

}
void something(){
  for (uint32_t i = 0; i <= 0xffff; i++) {
    Serial.print("*"); 
    Serial.println(count); 
  }
}