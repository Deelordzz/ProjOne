#include <Arduino.h>

/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */
 
<<<<<<< HEAD:ProjOne/src/main.cpp
=======
// Pin 13 has an LED connected on most Arduino boards.
// Pin 11 has the LED on Teensy 2.0
// Pin 6  has the LED on Teensy++ 2.0
// Pin 13 has the LED on Teensy 3.0
// give it a name:
int led = LED_BUILTIN;
>>>>>>> d4c86cfd6b1488a87c5721e8996082db2237e871:src/main.cpp

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
  delay(100);               // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
<<<<<<< HEAD:ProjOne/src/main.cpp
  delay(1000);               // wait for a second
  Serial.print("led value: ");
  Serial.println(led);
  Serial.print("# of bytes ");
  Serial.println((int)sizeof(led));
  int numberone = 2147483647;
  Serial.print("numberone = ");
  Serial.println(numberone);
  Serial.print("numberone + 1 = ");
  Serial.println(numberone + 1);
=======
  delay(100);               // wait for a second
>>>>>>> d4c86cfd6b1488a87c5721e8996082db2237e871:src/main.cpp
}