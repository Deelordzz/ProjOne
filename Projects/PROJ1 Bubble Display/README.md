 ## <center> **MG7013**

## <center>EMBEDDED SYSTEMS</center>

## <center>Frank Beinersdorf</center>

Wellington Institute of Technology
School of Engineering
May 19, 2018

## <center> Lourdrigo de Castro

Wellington Institute of Technology
School of Engineering

## Abstract

This project involves the use of a 4-digit seven segment numeric display to be controlled by a microcontroller. The goal is to come up with a system that will work as a stop watch. The number will be presented on a bubble display. The system will be controlled with a stop/start button and a reset button.

## Introduction

A bubble display is a tiny 4-digit seven segment numeric display. It is an electronic device that is used for visual presentation of numerical characters. It is mostly used as a simple display of a clock, stopwatch and other electronic device that display numbers.

The purpose of this report is to present a program that will be embedded in a microcontroller that will function as a minimal stopwatch. The bubble display will project the numerical information when a button is pressed and will stop the count when the same button is pressed again. Another button will be used to reset the display and start the time to zero.

The next chapter of this report discusses the methods and materials used to create a system that will function as a minimal stopwatch.

## **Material and Methods**

**_Hardware and Wiring_**

The  control circuit is composed of a microcontroller to control the bubble display. An Arduino Teensy 3.2 will be used as a microcontroller. Two pushbuttons are used to create input signals. One pushbutton is used to start and stop the stopwatch and the other pushbutton is to reset the stopwatch.

![enter image description here](https://i.ibb.co/qDhjwN8/fig-1.png)
_Figure 1: Schematic Diagram_

The bubble display with model no. HP QDSP-6064 was used in this project. It has 12 pins that represents all the segments and digits of the bubble display. Overall, there are 32 segments(anodes) and 4 digits(cathodes). Since it uses a common-cathode type LED which means all the LEDs in a given segment share one ground pin, 12 pins are enough to control the whole bubble display. Figure 1 shows the connections of all 12 pins to the microcontroller.

To protect the LED lights of the bubble display, a current limiting resistor is installed in series to pins of the segments(anode). This is to lessen the current that is entering the LED and to avoid the LED to burn-out too fast. Using Ohms law and the data from Figure 2, we can calculate for the value of the current limiting resistor for a source voltage of 3.3volts we get 340ohms. The nearest higher rated resistor value is 390ohms and are connected on each anodes of the bubble display.

![enter image description here](https://i.ibb.co/xfmsKD0/fig2.png)
_Figure 2: HP QDSP-6064 Datasheet_

On the cathode side of the bubble display, we used a MOSFET to sink the current to the ground. Though we can sink the current to the output pins of the microcontroller, it is safer to sink it to the ground. Four N-Channel MOSFET (IRLU8743) was used as a switch. Each source pin is connected to the ground, their drains are connected to each of the bubble display cathode and each gate is connected in series to a 1k ohms resistor going to the designated output pins of the microcontroller. The resistor is used to prevent gate ringing or noises that can switch the gate unnecessarily.

Another good engineering practice is to connect an external pull-down resistor from the gate voltage supply to ground. This will hold the logic signal near zero volts when there is no active device connected. On this project, it is not critical because the MOSFET will turn off when the its gate voltage is less than its threshold voltage.

![enter image description here](https://i.ibb.co/ys559cD/fig3.jpg)
_Figure 3: Circuit Connection on Breadboard_

All hardware are laid out on the breadboard. Following the schematic diagram, they are wired on their designated pins. Figure 3 shows the actual wiring of my bubble display minimal stopwatch system.

**_Software and Program Design_**

On this project, I have used a library made by Sparkfun Electronics. I have downloaded the SevSeg library using the PlatformIO extension on VScode. Since MOSFET will be used to sink the current of the cathodes of the bubble display, minor modification of the library is needed. Using Notepad++, I edited the SevSeg.cpp file. The value of the DigitOn and DigitOff were replaced to its opposite value. This will make the MOSFET function correctly as a normally close switch.

      //Assign input values to variables
      //mode is what the digit pins must be set at for it to be turned on. 0 for common cathode, 1 for common anode
      mode = mode_in;
      if(mode == COMMON_ANODE)
      {
        DigitOn = LOW;
        DigitOff = HIGH;
        SegOn = LOW;
        SegOff = HIGH;
      }
      else
      {
        DigitOn = HIGH;
        DigitOff = LOW;
        SegOn = HIGH;
        SegOff = LOW;
      }
_Code 1: SevSeg Library Modification_

I started the code by defining all the global parameters that will be used in the program. This also includes the pre-compilers Arduino.h and SevSeg.h. Using these pre-compilers, some parameters need not be defined since they are already defined in these libraries.

    /* Bubble-Display */
    /* Author: Lourdrigo de Castro*/
    
    #include <Arduino.h>
    #include "SevSeg.h"
    
    //Create an instance of the object.
    SevSeg myDisplay;
    
    //Create global variables 
    unsigned long OnTime = 0; // will store the last time the stopwatch is counting.
    unsigned long Stoptime = 0; // will store the last time the stopwatch is stopped.             
    bool ButtonOn = false; // use to detect if the button is ON
    int deciSecond = 0; // initial count in decisecond
    const uint32_t timeDebounce = 500; //debounce time
    uint8_t R_button = 23; // button connected to pin 23
    uint8_t SS_button = 2; // button connected to pin 2
    
    void ISR_Start_Stop(); // Start Stop function
    void ISR_Reset(); // Reset function
    void stopwatch(); // time counter function

_Code 2: Global Parameters_

The two pushbuttons that is physically connected and used as the Start/Stop and Reset button are defined as **SS_button** to pin 2 and **R_button** to pin 23 respectively. The **ButtonOn** which is used to call a function in this program is initialized as false. **deciSecond** which is the variable that will be projected to the bubble display is set to zero. **timeDebounce** is set to 500 because the signal noise was still observed using lower value. The functions **ISR_Start_Stop, ISR_Reset** and **stopwatch** is also defined in this part of the program code.

    void setup(){
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

_Code 3: void setup()_

The next part of the program initializes the pins where the hardwares are designated. The **R_button** and the **SS_button** are set as an input signals. The internal pullup resistor was also enabled for these buttons. **attachInterrupt** functions were used in this program to call the **ISR_Start_Stop** and **ISR_Reset** functions. The displayType was set as **COMMON_CATHODE** which was modified in the SevSeg library. The 12 pins of the bubble display were defined here and mapped to which microcontroller pins it is connected. Other variables were left as is and were part of the SevSeg library as default.

    void loop(){
    if (ButtonOn) { //check if the ButtonON is turned ON. 
      stopwatch(); // call the stopwatch function
        } else {
          Stoptime = millis() - OnTime; // time the stopwatch is stopped.
          char tempString[10]; // display the current time in decisecond.
          sprintf(tempString, "%4d", deciSecond);
          myDisplay.DisplayString(tempString, 4);
        }
     if (deciSecond == 9000){ //stopwatch will reset at 9000 decisecond or 15minutes
       ButtonOn = LOW;
       ISR_Reset();
      }
    }

_Code4: void loop()_

The **loop** function contains an ‘if else’ statement and an ‘if’ statement. The ‘if else’ statement is continuously checking the state of the variable **ButtonOn** which was initialized as false value. If the **ButtonOn** is true, the function **stopwatch** will be called else the **Stoptime** variable will start counting while displaying the present value of the **deciSecond**. On the other hand, the ‘if’ statement is checking the value of the **deciSecond**. Since this is a minimal stopwatch, I have decided to make it a 15minute stopwatch. If the **deciSecond** reach the value of 9000 or exactly 15minutes, the stopwatch will be reset back to zero.

    void stopwatch(){
    char tempString[10]; // display the current time in decisecond.
    sprintf(tempString, "%4d", deciSecond); 
    myDisplay.DisplayString(tempString, 4); 
    
    OnTime = (millis() -  Stoptime); // time the stopwatch is counting.
    deciSecond = (OnTime/100); // millisecond is converted to deciseconds.
    }

_Code 5: void stopwatch()_

Going back to the loop function, when the **ButtonOn** is true, the **stopwatch** function will be called. In this function, the variable **OnTime** will start to count and the **Stoptime** value will be equal to whatever value it is stopped the moment the **ButtonOn** becomes true. The **deciSecond** will be displayed into the bubble display. The value of the **deciSecond** is the value of **OnTime** and divided by 100 to convert the value which is in millisecond to decisecond. When the **ButtonOn** is false, the **OnTime** will stop counting and will carry the value to get the new **Stoptime** value so when the **ButtonOn** is true again it will start to count where it was left off unless it was reset.

    void ISR_Start_Stop(){
     uint32_t timeNewKeyPress = millis(); // set the time the SS_button is not pressed
     static uint32_t timeLastKeyPress = 0; //time the SS_button is pressed
     if ( timeNewKeyPress - timeLastKeyPress >= timeDebounce) { // equating to the timeDebounce              
        ButtonOn = !ButtonOn; //sets the state of the button everytime the SS_button is pressed
      }
        timeLastKeyPress = timeNewKeyPress; //reset the debouncing timer
    }

_Code 6: ISR_Start_Stop()_

The two connected buttons will call different functions when pressed. The **SS_button** will call the **ISR_Start_Stop** function. This function is responsible in setting the state of the variable **ButtonOn**. Every time it is pressed the value of the **ButtonOn** change to either true or false since this is declared as a bool integer. The **SS_button** signal is also being debounce in this function as shown in Code 6.

    void ISR_Reset(){
    uint32_t timeNewKeyPress = millis(); // set the time the R_button is not pressed
    static uint32_t timeLastKeyPress = 0; //time the R_button is pressed
    if ( timeNewKeyPress - timeLastKeyPress >= timeDebounce) { // equating to the timeDebounce               
      if (ButtonOn == LOW){ // check if the ButtonOn is OFF
          OnTime = 0; //Reset the OnTime to zero
          deciSecond = 0; //Rreset the deciSecond to zero
        }
      }
        timeLastKeyPress = timeNewKeyPress; //reset the debouncing timer
    }

_Code 7: ISR_Reset()_

The other button which is the **R_button** will call the function **ISR_Reset** when pressed. This function will check the state of the **ButtonOn** if it is false. If the **ButtonOn** is false, the values of **OnTime** and **deciSecond** variables will be set zero. Nothing will happen if the **ButtonOn** is true. This function also debounces the signal of the R_button.

## **Results**

All hardwares are functioning as programmed. The **SS_button** starts and stops the stopwatch while the **R_button** resets the stopwatch back to zero. The counting before the decimal point is in seconds with a timing of 10 deciseconds.

Three trials were conducted in testing this embedded system. The first trial was stopped at 6seconds. The values are identical with the bubble display and an external stopwatch.

![enter image description here](https://i.ibb.co/zJMh6Bw/fig11.png)

_Figure 4: First Trial_

The second and third trial also resulted an almost identical value from the bubble display and an external stopwatch. The time log is at 23.8 seconds and 59.9 seconds respectively.

![enter image description here](https://i.ibb.co/vLNzXhN/fig12.jpg)

_Figure 5: Second Trial_

![fig13](https://i.ibb.co/FzS8gn7/fig13.png)

_Figure 6: Third Trial_

## **Discussion**

This project shows us how a microcontroller can control multiple LED’s and present it as a numerical character on a device such as a bubble display. In this project, I use it to control the bubble display and to make it function as a stopwatch. Using the internal time function of the microcontroller, the timing of the stopwatch becomes accurate. I was able to eliminate the use of delay function that can make the other function of the program idle. 

On a larger application, this can be use on an LED show that can project images and lights up a big event. This is truly one of an amazing purpose of an embedded system.     

## **Reference**

Pjrc.com. (2019). Teensy 3.2 & 3.1: New Features. [online] Available at: https://www.pjrc.com/teensy/teensy31.html [Accessed 27 Apr. 2019].

Forum.arduino.cc. (2019). Arduino Forum - Index. [online] Available at: https://forum.arduino.cc/ [Accessed 17 Apr. 2019].

(4-digit), B. (2019). _Bubble Display - 7-Segment (4-digit) - COM-12710 - SparkFun Electronics_. [online] Sparkfun.com. Available at: https://www.sparkfun.com/products/retired/12710 [Accessed 14 May 2019].

AG, I. (2019). _IRLU8743 - Infineon Technologies_. [online] Infineon.com. Available at: https://www.infineon.com/cms/en/product/power/mosfet/20v-300v-n-channel-power-mosfet/20v-30v-n-channel-power-mosfet/irlu8743/ [Accessed 14 May 2019].

ElProCus - Electronic Projects for Engineering Students. (2019). _What is the MOSFET: Basics, Working Principle and Applications_. [online] Available at: https://www.elprocus.com/mosfet-as-a-switch-circuit-diagram-free-circuits/ [Accessed 14 May 2019].

## **Appendix**

    /* Bubble-Display */
    /* Author: Lourdrigo de Castro*/
    
    #include <Arduino.h>
    #include "SevSeg.h"
    
    //Create an instance of the object.
    SevSeg myDisplay;
    
    //Create global variables 
    unsigned long OnTime = 0; // will store the last time the stopwatch is counting.
    unsigned long Stoptime = 0; // will store the last time the stopwatch is stopped.             
    bool ButtonOn = false; // use to detect if the button is ON
    int deciSecond = 0; // initial count in decisecond
    const uint32_t timeDebounce = 500; //debounce time
    uint8_t R_button = 23; // button connected to pin 23
    uint8_t SS_button = 2; // button connected to pin 2
    
    void ISR_Start_Stop(); // Start Stop function
    void ISR_Reset(); // Reset function
    void stopwatch(); // time counter function
    
    void setup(){
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
    if (ButtonOn) { //check if the ButtonON is turned ON. 
      stopwatch(); // call the stopwatch function
        } else {
          Stoptime = millis() - OnTime; // time the stopwatch is stopped.
          char tempString[10]; // display the current time in decisecond.
          sprintf(tempString, "%4d", deciSecond);
          myDisplay.DisplayString(tempString, 4);
        }
     if (deciSecond == 9000){ //stopwatch will reset at 9000 decisecond or 15minutes
       ButtonOn = LOW;
       ISR_Reset();
      }
    }
    
    void stopwatch(){
    char tempString[10]; // display the current time in decisecond.
    sprintf(tempString, "%4d", deciSecond); 
    myDisplay.DisplayString(tempString, 4); 
    
    OnTime = (millis() -  Stoptime); // time the stopwatch is counting.
    deciSecond = (OnTime/100); // millisecond is converted to deciseconds.
    }
    
    void ISR_Start_Stop(){
     uint32_t timeNewKeyPress = millis(); // set the time the SS_button is not pressed
     static uint32_t timeLastKeyPress = 0; //time the SS_button is pressed
     if ( timeNewKeyPress - timeLastKeyPress >= timeDebounce) { // equating to the timeDebounce              
        ButtonOn = !ButtonOn; //sets the state of the button everytime the SS_button is pressed
      }
        timeLastKeyPress = timeNewKeyPress; //reset the debouncing timer
    }
             
    void ISR_Reset(){
    uint32_t timeNewKeyPress = millis(); // set the time the R_button is not pressed
    static uint32_t timeLastKeyPress = 0; //time the R_button is pressed
    if ( timeNewKeyPress - timeLastKeyPress >= timeDebounce) { // equating to the timeDebounce               
      if (ButtonOn == LOW){ // check if the ButtonOn is OFF
          OnTime = 0; //Reset the OnTime to zero
          deciSecond = 0; //Rreset the deciSecond to zero
        }
      }
        timeLastKeyPress = timeNewKeyPress; //reset the debouncing timer
    }

