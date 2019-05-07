#include <Arduino.h>
/*
 March 6, 2014
 Spark Fun Electronics
 Nathan Seidle
 Updates by Joel Bartlett
 
 This code is originally based Dean Reading's Library deanreading@hotmail.com
 http://arduino.cc/playground/Main/SevenSegmentLibrary
 He didn't have a license on it so I hope he doesn't mind me making it public domain: 
 This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).
 
 This sketch provides a simple counter example for the HP Bubble display from SparkFun.
 https://www.sparkfun.com/products/12710
 
 Pinout for HP Bubble Display:
 1:  Cathode 1
 2:  Anode E
 3:  Anode C
 4:  Cathode 3
 5:  Anode dp
 6:  Cathode 4
 7:  Anode G
 8:  Anode D
 9:  Anode F
 10: Cathode 2
 11: Anode B
 12: Anode A
 */

#include "SevSeg.h"

//Create an instance of the object.
SevSeg myDisplay;

//Create global variables
unsigned long timer;
uint8_t buttonState;             // the current reading from the input pin
uint8_t buttonState2;
bool keyPressed = false;
uint32_t timeDebounce = 500;
int deciSecond = 0;
int display = 0;
uint8_t R_button = 23; // button connected to pin 12
uint8_t SS_button = 2; // button connected to pin 12

void ISR_Start_Stop();
void ISR_Reset();
void stopwatch();

void setup()
{
  pinMode(R_button, INPUT_PULLUP); // set the digital pin as input and enable internal pullup resistor
  pinMode(SS_button, INPUT_PULLUP); // set the digital pin as input and enable internal pullup resistor
  attachInterrupt(digitalPinToInterrupt(SS_button), ISR_Start_Stop, FALLING); // attached interrupt to call ISR_button()
  attachInterrupt(digitalPinToInterrupt(R_button), ISR_Reset, FALLING); // attached interrupt to call ISR_button()
  int displayType = COMMON_CATHODE; //Your display is either common cathode or common anode

  
    //This pinout is for a bubble dispaly
       //Declare what pins are connected to the GND pins (cathodes)
       int digit1 = 12; //Pin 1
       int digit2 = 14; //Pin 10
       int digit3 = 11; //Pin 4
       int digit4 = 10; //Pin 6
       
       //Declare what pins are connected to the segments (anodes)
       int segA = 15; //Pin 12
       int segB = 16; //Pin 11
       int segC = 8; //Pin 3
       int segD = 18; //Pin 8
       int segE = 9; //Pin 2
       int segF = 17; //Pin 9
       int segG = 19; //Pin 7
       int segDP= 7; //Pin 5
   
  int numberOfDigits = 4; //Do you have a 1, 2 or 4 digit display?

  myDisplay.Begin(displayType, numberOfDigits, digit1, digit2, digit3, digit4, segA, segB, segC, segD, segE, segF, segG, segDP);
  
  myDisplay.SetBrightness(100); //Set the display to 100% brightness level

  timer = millis();
}

void loop()
{
 if (keyPressed)
  {
   stopwatch();
  } else {
      char tempString[10];
      sprintf(tempString, "%4d", deciSecond);
      myDisplay.DisplayString(tempString, 4);
     }
    
} 
 
void ISR_Start_Stop()
{
 uint32_t timeNewKeyPress = millis();
 static uint32_t timeLastKeyPress = 0;
 if ( timeNewKeyPress - timeLastKeyPress >= timeDebounce)
    {              
     if (SS_button != buttonState) {
          keyPressed = !keyPressed;
      }
      timeLastKeyPress = timeNewKeyPress;
    }
  }
          

void stopwatch(){
  //Example ways of displaying a decimal number
  char tempString[10]; //Used for sprintf
  sprintf(tempString, "%4d", deciSecond); //Convert deciSecond into a string that is right adjusted
  //Produce an output on the display
  myDisplay.DisplayString(tempString, 4); //(numberToDisplay, decimal point location in binary number [4 means the third digit])

  //Check if 10ms has elapsed
  if (millis() - timer >= 100)
  {
    timer = millis();
    deciSecond++;
  }
}

void ISR_Reset()
{
 uint32_t timeNewKeyPress = millis();
 static uint32_t timeLastKeyPress = 0;
 if ( timeNewKeyPress - timeLastKeyPress >= timeDebounce)
    {              
     if (R_button != buttonState2) {
          }
      timeLastKeyPress = timeNewKeyPress;
    }
  }
