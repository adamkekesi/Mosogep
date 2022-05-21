#include <Arduino.h>
#include "FrequencyMeasurement.h"
#include "stdlib.h"
#include "StateMachine.h"

FrequencyMeasurement freq = FrequencyMeasurement();

unsigned long ULONG_MAX = 0x7fffffffL * 2UL + 1UL;

int freqPin = 5;
int input8Pin = 8;
int input7Pin = 7;
int input6Pin = 6;
int output12Pin = 12;
int output11Pin = 11;
int output10Pin = 10;

unsigned long disableTime = 2 * 60 * pow(10, 6);
unsigned long duration6 = 1 * 60 * pow(10, 6);

int freq1 = 43;
int freq2 = 43.5;
int freq3 = 42.5;

int disableFreq12 = freq2;
int disableFreq11 = freq2;
int disableFreq10 = freq1;

unsigned long disableTime12;

int out12 = LOW;
int out11 = LOW;
int out10 = LOW;

bool is8Enabled = false;

bool is8High = false;
bool is7High = false;

unsigned long cooldown = pow(10, 6);

unsigned long cooldown8 = -1;
unsigned long cooldown7 = -1;

unsigned long lastTime = -1; // microsec

unsigned long printTime = 0;

StateMachine *stateMachine;

void setup()
{
  stateMachine = new StateMachine(disableTime, disableTime, disableFreq10, disableFreq11, duration6);
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

  unsigned long diff = time - lastTime;
  lastTime = time;
  return diff;
}

void loop()
{
  unsigned long timeSinceLastLoop = CalculateTimeSinceLastLoop();
  freq.Tick(digitalRead(freqPin));

  double frequency = freq.frequency;

  int input8 = digitalRead(input8Pin);
  int input7 = digitalRead(input7Pin);
  int input6 = digitalRead(input6Pin);

  /* // 7 és 8
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
    disableFreq12 = freq3;
    disableFreq11 = freq3;
  }
  else
  {
    disableFreq12 = freq2;
    disableFreq11 = freq2;
  } */

  stateMachine->disableFreq10 = disableFreq10;
  stateMachine->disableFreq11 = disableFreq11;

  stateMachine->Tick(timeSinceLastLoop, input7, input6, frequency);

  out11 = stateMachine->GetStateOf11();
  out10 = stateMachine->GetStateOf10();

  // 8
  if (is8Enabled)
  {
    disableTime12 += timeSinceLastLoop;
  }
  if (!is8Enabled && input8 == HIGH)
  {
    is8Enabled = true;
    out12 = HIGH;
  }
  if (frequency <= disableFreq12 || disableTime12 >= disableTime)
  {
    Serial.print("12: ");
    Serial.print(frequency);
    Serial.print("<=");
    Serial.println(disableFreq12);
    is8Enabled = false;
    out12 = LOW;
    disableTime12 = 0;
  }

  digitalWrite(output12Pin, out12);
  digitalWrite(output11Pin, out11);
  digitalWrite(output10Pin, out10);

  printTime += timeSinceLastLoop;

  if (printTime > pow(10, 6))
  {
    Serial.print("freq:");
    Serial.println(frequency);

    printTime = 0;
  }
}
