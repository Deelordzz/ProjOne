#include <Arduino.h>

uint8_t ledlight = 0; // LED connected to pin 0
uint8_t button = 12; // button connected to pin 12
uint8_t ledState = LOW; // used to set the LED
unsigned long previousMillis = 0; // will store last time LED was updated 
float frequency; // 1/period; frequency value
unsigned long value = 0; // value read from the potentiometer
float period = 0; // value output in millisecond for the interval of the LED blink
const uint32_t DEBOUNCETIME_MS = 100; //debounce time

void ISR_button(); // button interrupt function

void setup() {
  pinMode(ledlight, OUTPUT); // set the digital pin as output
  pinMode(button, INPUT_PULLUP); // set the digital pin as input and enable internal pullup resistor 
  attachInterrupt(digitalPinToInterrupt(button), ISR_button, FALLING); // attached interrupt to call ISR_button()
  Serial.begin(9600);

}

void loop() {
  unsigned long currentMillis = millis(); // set the current time in millisecond
  value = analogRead(A0); // Read the analog value on pin A0
  period = (map(value, 0 , 1023, 5 , 100)); // mapping of the potentiometer
  frequency = (1/period) * 500; // frequency @ 50% duty cycle
  // check to see if its time to blink
  if (currentMillis - previousMillis >= period) { // comparing the difference between the cuurent time and the last time the LED blink to the value of the period
    if (ledState == LOW) {                        // turn off the LED if its ON and vice versa
      ledState = HIGH;
    } else {
      ledState = LOW;
    }
    previousMillis = currentMillis; // save the last time the LED blink
    digitalWrite(ledlight, ledState); // set the LED with the variable ledstate
  }
}

void ISR_button() {
  static uint32_t previousTime_ms = 0; // button is pressed
  uint32_t currentTime_ms = millis(); // set the current time the button is not pressed

  if((currentTime_ms - previousTime_ms) > DEBOUNCETIME_MS){ // comparing the difference between the current time and the last time the button is pressed to the set debounce time
    Serial.print(period); // display the period and frequency
    Serial.print("ms ");
    Serial.print(frequency);
    Serial.println("Hz ");
    Serial.flush();
   } 
  previousTime_ms = currentTime_ms; // reset the debouncing timer
}
    