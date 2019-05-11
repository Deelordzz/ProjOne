/*

 */
#include <Arduino.h>
#include "SevSeg.h"

//Create an instance of the object.
SevSeg myDisplay;

//Create global variables 
unsigned long Elapsetime = 0; // will store the last time the stopwatch is counting.
unsigned long Stoptime = 0; // will store the last time the stopwatch is stopped.             
bool ButtonOn = false; // use to detect if the button is ON
int deciSecond = 0; // initial count in decisecond
const uint32_t timeDebounce = 500; //debounce time
uint8_t buttonState; // use to set the button  
uint8_t R_button = 23; // button connected to pin 23
uint8_t SS_button = 2; // button connected to pin 2

void ISR_Start_Stop(); // Start Stop function
void ISR_Reset(); // Reset function
void stopwatch(); // time counter function

void setup()
{
  pinMode(R_button, INPUT_PULLUP); // set the digital pin as input and enable internal pullup resistor
  pinMode(SS_button, INPUT_PULLUP); // set the digital pin as input and enable internal pullup resistor
  attachInterrupt(digitalPinToInterrupt(SS_button), ISR_Start_Stop, FALLING); // attached interrupt to call ISR_Start_Stop()
  attachInterrupt(digitalPinToInterrupt(R_button), ISR_Reset, FALLING); // attached interrupt to call ISR_Reset()
  int displayType = COMMON_CATHODE; //Your display is either common cathode or common anode
  
  //This pinout is for a bubble display
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
   
  int numberOfDigits = 4; // 4 digit display

  myDisplay.Begin(displayType, numberOfDigits, digit1, digit2, digit3, digit4, segA, segB, segC, segD, segE, segF, segG, segDP);
  
  myDisplay.SetBrightness(100); //Set the display to 100% brightness level

}

void loop(){
 if (ButtonOn) { 
   stopwatch();
  } else {
      Stoptime = millis() - Elapsetime; 
      char tempString[10];
      sprintf(tempString, "%4d", deciSecond);
      myDisplay.DisplayString(tempString, 4);
    }
} 
 
void ISR_Start_Stop(){
 uint32_t timeNewKeyPress = millis();
 static uint32_t timeLastKeyPress = 0;
 if ( timeNewKeyPress - timeLastKeyPress >= timeDebounce) {              
  if (SS_button != buttonState) {
    ButtonOn = !ButtonOn;
    }
    timeLastKeyPress = timeNewKeyPress;
  }
}
         
void stopwatch(){
char tempString[10]; //Used for sprintf
sprintf(tempString, "%4d", deciSecond); //Convert deciSecond into a string that is right adjusted
myDisplay.DisplayString(tempString, 4); //(numberToDisplay, decimal point location in binary number [4 means the third digit])

Elapsetime = (millis() -  Stoptime);
deciSecond = (Elapsetime/100); // add 1 per 10ms
}

void ISR_Reset(){
uint32_t timeNewKeyPress = millis();
static uint32_t timeLastKeyPress = 0;
if ( timeNewKeyPress - timeLastKeyPress >= timeDebounce) {              
  if (SS_button != buttonState) {
    if (ButtonOn == LOW){
      Elapsetime = 0;
      deciSecond = 0;
      }
    }
    timeLastKeyPress = timeNewKeyPress;
  }
}
