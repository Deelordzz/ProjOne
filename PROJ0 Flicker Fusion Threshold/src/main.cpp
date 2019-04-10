#include <Arduino.h>

uint32_t ledlight = 0;
uint32_t button = 12;
uint32_t ledState = LOW;
unsigned long previousMillis = 0;
float interval;
float value = 0;
float frequency = 0;
const uint32_t DEBOUNCETIME_MS = 100;
void ISR();

void setup() {
  // put your setup code here, to run once:
  pinMode(ledlight, OUTPUT);
  pinMode(button, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(button), ISR, FALLING);
  Serial.begin(9600);

}

void loop() {
   unsigned long currentMillis = millis();
   value = (float)analogRead(A0);
   frequency = (map(value, 0 , 1024, 0 , 60));
   interval = (1/frequency) * 1000;

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }

    digitalWrite(ledlight, ledState);
   
  }
}

void ISR() {
     previousTime_ms = currentTime_ms;
  Serial.print(frequency);
    Serial.println("Hz ");
    Serial.flush();
  }
}