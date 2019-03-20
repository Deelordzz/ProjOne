#include <Arduino.h>

int32_t InputNumber = -0xff; //signed integer at 16bit
 

void setup() {                
  // initialize the digital pin as an output.
  Serial.begin(9600);
   
}

// the loop routine runs over and over again forever:
void loop() {
  delay(1000);
  Serial.print("Dec: ");
  Serial.println(InputNumber);
  Serial.print("Bin: ");
  Serial.println(InputNumber, BIN);
  Serial.print("Oct: ");
  Serial.println(InputNumber, OCT);
  Serial.print("Hex: ");
  Serial.println(InputNumber, HEX);
 
}