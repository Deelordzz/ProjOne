#include <Arduino.h>

/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */
 

//int led = 13;
//int led = LED_BUILTIN;
//#define led 13 
//unsigned int led = 13;
uint32_t led = 13; //Unsigned integer at 32bit
 
// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  Serial.begin(9600);
  pinMode(led, OUTPUT);      
}

// the loop routine runs over and over again forever:
void loop() {
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);               // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);               // wait for a second
  Serial.print("led value: ");
  Serial.println(led);
  Serial.print("# of bytes ");
  Serial.println((int)sizeof(led));
  int numberone = 2147483647; // Overflow test
  Serial.print("numberone = ");
  Serial.println(numberone);
  Serial.print("numberone + 1 = ");
  Serial.println(numberone + 1);
}