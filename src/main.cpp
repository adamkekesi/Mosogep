#include <Arduino.h>
#include "FrequencyMeasurement.h"

FrequencyMeasurement freq1 = FrequencyMeasurement();

void setup()
{
  pinMode(5, INPUT);
  Serial.begin(9600);
}

void loop()
{
  freq1.Tick(digitalRead(5));
  
  
  

}