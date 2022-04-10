#include <Arduino.h>
#include "FrequencyMeasurement.h"
#include "stdlib.h"

FrequencyMeasurement freq = FrequencyMeasurement();

unsigned long ULONG_MAX = 0x7fffffffL * 2UL + 1UL;

int freqPin = 5;
int input8Pin = 8;
int input7Pin = 7;
int input6Pin = 6;
int output12Pin = 12;
int output11Pin = 11;
int output10Pin = 10;

unsigned long disableTime = 0.5 * 60 * pow(10, 6);
int duration6 = 1 * 60 * pow(10, 6);

int freq1 = 43;
int freq2 = 43.5;
int freq3 = 42.5;

int disableFreq8 = freq2;
int disableFreq7 = freq2;
int disableFreq6 = freq1;

unsigned long disableTime8;
unsigned long disableTime7;
unsigned long disableTime6;

unsigned long time6 = -1;

int out12 = LOW;
int out11 = LOW;
int out10 = LOW;

bool is7Overridden = false;

bool is8Enabled = false;
bool is7Enabled = false;
bool is6Enabled = false;

bool is8High = false;
bool is7High = false;

int cooldown = pow(10, 6);

unsigned long cooldown8 = -1;
unsigned long cooldown7 = -1;

unsigned long lastTime = -1; // microsec

unsigned long printTime = 0;

void setup()
{
  pinMode(freqPin, INPUT);
  pinMode(input8Pin, INPUT);
  pinMode(input7Pin, INPUT);
  pinMode(input6Pin, INPUT);
  pinMode(output12Pin, OUTPUT);
  pinMode(output11Pin, OUTPUT);
  pinMode(output10Pin, OUTPUT);

  Serial.begin(9600);
}

unsigned long CalculateTimeSinceLastLoop()
{
  unsigned long time = micros();

  if (lastTime == -1)
  {
    lastTime = time;
    return 0;
  }

  // overflow
  if (time < lastTime)
  {
    lastTime = time;
    return ULONG_MAX - lastTime + time;
  }

  lastTime = time;
  return time - lastTime;
}

void loop()
{
  unsigned long timeSinceLastLoop = CalculateTimeSinceLastLoop();
  freq.Tick(digitalRead(freqPin));

  double frequency = freq.frequency;

  double input8 = digitalRead(input8Pin);
  double input7 = digitalRead(input7Pin);
  double input6 = digitalRead(input6Pin);

  // 8
  if (is8Enabled)
  {
    disableTime8 += timeSinceLastLoop;
  }
  if (!is8Enabled && input8 == HIGH)
  {
    is8Enabled = true;
    out12 = HIGH;
  }
  if (frequency <= disableFreq8 || disableTime8 >= disableTime)
  {
    is8Enabled = false;
    out12 = LOW;
    disableTime8 = 0;
  }

  // 7
  if (is7Enabled)
  {
    disableTime7 += timeSinceLastLoop;
  }
  if (!is7Enabled && input7 == HIGH)
  {
    is7Enabled = true;
    if (is7Overridden)
    {
      out11 = LOW;
    }
    else
    {
      out11 = HIGH;
    }
  }
  if (!is7Overridden)
  {
    if (frequency <= disableFreq7 || disableTime7 >= disableTime)
    {
      is7Enabled = false;
      out11 = LOW;
      disableTime7 = 0;
    }
  }

  // 6
  if (is6Enabled)
  {
    disableTime6 += timeSinceLastLoop;
  }

  // még nem telt le az 5p
  if (time6 >= 0)
  {
    time6 -= timeSinceLastLoop;
  }
  else // már letelt az 5p, vagy nincs is a 6os bekapcsolva
  {
    time6 = -1;
  }

  if (time6 < 0 && input6 == HIGH)
  {
    time6 = duration6;
    is7Overridden = true;
  }
  if (frequency <= disableFreq6 || disableTime6 >= disableTime)
  {
    disableTime6 = 0;
    disableTime7 = 0;
    is6Enabled = false;
    is7Enabled = false;
    if (time6 < 0)
    {
      is7Overridden = false;
    }
  }

  // 7 és 8
  if (cooldown8 > 0)
  {
    cooldown8 -= timeSinceLastLoop;
  }
  else
  {
    is8High = false;
    cooldown8 = -1;
  }

  if (cooldown7 > 0)
  {
    cooldown7 -= timeSinceLastLoop;
  }
  else
  {
    is7High = false;
    cooldown7 = -1;
  }

  if (input8 == HIGH)
  {
    is8High = true;
    cooldown8 = cooldown;
  }
  if (input7 == HIGH)
  {
    is7High = true;
    cooldown7 = cooldown;
  }

  if ((input8 && is7High) || (input7 && is8High))
  {
    disableFreq8 = freq3;
    disableFreq7 = freq3;
  }
  else
  {
    disableFreq8 = freq2;
    disableFreq7 = freq2;
  }

  digitalWrite(output12Pin, out12);
  digitalWrite(output11Pin, out11);
  digitalWrite(output10Pin, out10);
  printTime += timeSinceLastLoop;
  if (printTime > pow(10, 6))
  {
    Serial.println(frequency);
    printTime = 0;
  }
}
