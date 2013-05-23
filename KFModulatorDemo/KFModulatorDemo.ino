/*
  KFModulatorDemo. - An Arduino sketch to demonstrate the KFModulator library
  Created by Roland Prähofer, May 17th, 2013
  This software is public domain.
  For peaceful application only
 */
#include <KFModulator.h>

KFModulator modulator; 
int ledPin = 9;      // led connected to digital pin 13

void setup() {
 pinMode(ledPin, OUTPUT); // declare the ledPin as as OUTPUT
 unsigned long kFrames[] = {0,700,1400,1500,1600,1700,1800,1900,2000,2100};
 int kfValues[] = {0,255,0,200,0,200,0,200,0,0};
 modulator.setKFrames(&kFrames[0],10);
 modulator.setKFValues(&kfValues[0],10);
 modulator.setLoop(true);
 modulator.start(millis());
}

void loop() {
  if(modulator.isRunning()){
    int res=modulator.update(millis()); 
    analogWrite(ledPin,res);
  }
  delay(10);  // delay to avoid overloading the serial port buffer
}

