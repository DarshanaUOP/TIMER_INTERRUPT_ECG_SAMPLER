#include <TimerOne.h>
#include <Wire.h>

int ECGdata ;

float t ;
double f = 250; //Hz - sampling frequency
String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

void setup(void) {
  Serial.begin(9600);

  pinMode(led, OUTPUT);
  setSamplingFreq(f);
  Serial.begin(9600);
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
}
void setSamplingFreq(double f) {
  t = (1 / f) * 1000000;
  Timer1.initialize(t);
  Timer1.attachInterrupt(readSensor); // Read data from the circuit in every sampling time.
}

void readSensor(void) {
  ECGdata = analogRead(1);            // Connect analog pin 1 to the circuit out
  Serial.println(ECGdata);
}
void loop() {
  // print the string when a newline arrives:
  if (stringComplete) {
    Serial.println(inputString);
    // clear the string:
    inputString = "";
    stringComplete = false;
  }
}

void serialEvent() {
  while (Serial.available()) {
    noInterrupts();
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
  
  setSamplingFreq(inputString.toDouble());
  interrupts();
}
