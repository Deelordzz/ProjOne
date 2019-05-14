**MG7013**

**EMBEDDED SYSTEMS**

**Frank Beinersdorf**

Wellington Institute of Technology

School of Engineering

April 28, 2018

**Lourdrigo de Castro**

Wellington Institute of Technology

School of Engineering

**Abstract**

This project involves the use of a 4-digit seven segment numeric display to be controlled by a microcontroller. The goal is to come up with a system that will work as a stop watch. The number will be presented on a bubble display. The system will be controlled with a stop/start button and a reset button.

**Introduction**

A bubble display is a tiny 4-digit seven segment numeric display. It is an electronic device that is used for visual presentation of numerical characters. It is mostly used as a simple display of a clock, stopwatch and other electronic device that display numbers.

The purpose of this report is to present a program that will be embedded in a microcontroller that will function as a minimal stopwatch. The bubble display will project the numerical information when a button is pressed and will stop the count when the same button is pressed again. Another button will be used to reset the display and start the time to zero.

The next chapter of this report discusses the methods and materials used to create a system that will function as a minimal stopwatch.

**Material and Methods**

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

_Figure 4: SevSeg Library Modification_

I started the code by defining all the global parameters that will be used in the program. This also includes the pre-compilers Arduino.h and SevSeg.h. Using these pre-compilers, some parameters need not be defined since they are already defined in these libraries.

_Figure 5: Global Parameters_

The two pushbuttons that is physically connected and used as the Start/Stop and Reset button are defined as **SS_button** to pin 2 and **R_button** to pin 23 respectively. The **ButtonOn** which is used to call a function in this program is initialized as false. **deciSecond** which is the variable that will be projected to the bubble display is set to zero. **timeDebounce** is set to 500 because the signal noise was still observed using lower value. The functions **ISR_Start_Stop, ISR_Reset** and **stopwatch** is also defined in this part of the program code.

_Figure 6: void setup()_

The next part of the program initializes the pins where the hardwares are designated. The **R_button** and the **SS_button** are set as an input signals. The internal pullup resistor was also enabled for these buttons. **attachInterrupt** functions were used in this program to call the **ISR_Start_Stop** and **ISR_Reset** functions. The displayType was set as **COMMON_CATHODE** which was modified in the SevSeg library. The 12 pins of the bubble display were defined here and mapped to which microcontroller pins it is connected. Other variables were left as is and were part of the SevSeg library as default.

_Figure 7: void loop()_

The **loop** function contains an ‘if else’ statement and an ‘if’ statement. The ‘if else’ statement is continuously checking the state of the variable **ButtonOn** which was initialized as false value. If the **ButtonOn** is true, the function **stopwatch** will be called else the **Stoptime** variable will start counting while displaying the present value of the **deciSecond**. On the other hand, the ‘if’ statement is checking the value of the **deciSecond**. Since this is a minimal stopwatch, I have decided to make it a 15minute stopwatch. If the **deciSecond** reach the value of 9000 or exactly 15minutes, the stopwatch will be reset back to zero.

_Figure 8: void stopwatch()_

Going back to the loop function, when the **ButtonOn** is true, the **stopwatch** function will be called. In this function, the variable **OnTime** will start to count and the **Stoptime** value will be equal to whatever value it is stopped the moment the **ButtonOn** becomes true. The **deciSecond** will be displayed into the bubble display. The value of the **deciSecond** is the value of **OnTime** and divided by 100 to convert the value which is in millisecond to decisecond. When the **ButtonOn** is false, the **OnTime** will stop counting and will carry the value to get the new **Stoptime** value so when the **ButtonOn** is true again it will start to count where it was left off unless it was reset.

_Figure 9: ISR_Start_Stop()_

The two connected buttons will call different functions when pressed. The **SS_button** will call the **ISR_Start_Stop** function. This function is responsible in setting the state of the variable **ButtonOn**. Every time it is pressed the value of the **ButtonOn** change to either true or false since this is declared as a bool integer. The **SS_button** signal is also being debounce in this function as shown on Figure 9.

_Figure 10: ISR_Reset()_

The other button which is the **R_button** will call the function **ISR_Reset** when pressed. This function will check the state of the **ButtonOn** if it is false. If the **ButtonOn** is false, the values of **OnTime** and **deciSecond** variables will be set zero. Nothing will happen if the **ButtonOn** is true. This function also debounces the signal of the R_button.

**Results**

All hardwares are functioning as programmed. The **SS_button** starts and stops the stopwatch while the **R_button** resets the stopwatch back to zero. The counting before the decimal point is in seconds with a timing of 10 deciseconds.

Three trials were conducted in testing this embedded system. The first trial was stopped at 6seconds. The values are identical with the bubble display and an external stopwatch.
![enter image description here](https://i.ibb.co/zJMh6Bw/fig11.png)
_Figure 11: First Trial_

The second and third trial also resulted an almost identical value from the bubble display and an external stopwatch. The time log is at 23.8 seconds and 59.9 seconds respectively.
![enter image description here](https://i.ibb.co/vLNzXhN/fig12.jpg)
_Figure 12: Second Trial_
![fig13](https://i.ibb.co/FzS8gn7/fig13.png)
_Figure 13: Third Trial_

**Discussion**

Every single person has their own threshold in identifying if a light is flickering. It is subjective to everyone who is seeing the light source. The light appears to be stable at higher frequency while the flashing light becomes noticeable at lower frequency.

Using an embedded system, I measured my own threshold in identifying the LED flickering. I can still see the light flashing on and off at 50 Hz and becomes steady beyond this frequency. Since the threshold is subjective to other people, they might have a different result. People with higher threshold are most likely to experience eye strain or headache on normal lighted places.

This is a simple yet powerful device that uses an embedded system. It is very useful in everyday lives. This project can be use in medical field for analysis of a person medical condition or in researched and development to improve consumer devices. This is only one thing out of a million things that an embedded system can do for us.

**Reference**

En.wikipedia.org. (2019). Flicker fusion threshold. [online] Available at: https://en.wikipedia.org/wiki/Flicker_fusion_threshold [Accessed 13 Apr. 2019].

Pjrc.com. (2019). Teensy 3.2 & 3.1: New Features. [online] Available at: https://www.pjrc.com/teensy/teensy31.html [Accessed 27 Apr. 2019].

Forum.arduino.cc. (2019). Arduino Forum - Index. [online] Available at: https://forum.arduino.cc/ [Accessed 17 Apr. 2019].

Ccohs.ca. (2019). (none). [online] Available at: https://www.ccohs.ca/oshanswers/ergonomics/lighting_flicker.html [Accessed 27 Apr. 2019].

Comparing


> Written with [StackEdit](https://stackedit.io/).
