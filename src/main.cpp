#include <Arduino.h>
#include "FrequencyMeasurement.h"

FrequencyMeasurement freq1 = FrequencyMeasurement();

void setup()
{
  pinMode(A0, INPUT);
  Serial.begin(9600);
}

void loop()
{
  freq1.Tick(digitalRead(A0));
  if (freq1.frequency > 0)
  {
    Serial.println(freq1.frequency);
  }
  
  

}