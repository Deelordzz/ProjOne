#include <Arduino.h>

int8_t a = 4; //signed integer at 8bit
int8_t b = -8;
int8_t c = 0;

void setup() {                
  // initialize the digital pin as an output.
  Serial.begin(9600);
   
}

// the loop routine runs over and over again forever:
void loop() {
  c = a - b;
  delay(1000);
  Serial.print("a: ");
  Serial.println(a, BIN);
  Serial.print("b: ");
  Serial.println(b, BIN);
  Serial.print("c: ");
  Serial.println(c, BIN);

 
}