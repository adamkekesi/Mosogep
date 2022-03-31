#include <Arduino.h>
#ifndef FrequencyMeasurement_h
#define FrequencyMeasurement_h

class FrequencyMeasurement
{
private:
    int lastMeasurement = -1;
    int changeCount;
    unsigned long cycleStartTime; // microseconds

public:
    double frequency = -2;

    FrequencyMeasurement();
    ~FrequencyMeasurement();

    void Tick(int digitalMeasurement);
};

FrequencyMeasurement::FrequencyMeasurement()
{
}

FrequencyMeasurement::~FrequencyMeasurement()
{
}

void FrequencyMeasurement::Tick(int digitalMeasurement)
{
    if (lastMeasurement == -1)
    {
        lastMeasurement = digitalMeasurement;
        return;
    }

    if (digitalMeasurement != lastMeasurement)
    {
        changeCount++;
    }

    if (changeCount == 2)
    {
        changeCount = 0;
        unsigned long currentTime = micros();
        if (frequency == -2)
        {
            frequency = -1;
        }
        else
        {
            unsigned long timeElapsed = currentTime - cycleStartTime;
            if (timeElapsed > 0)
            {
                frequency = pow(10, 6) / timeElapsed;
            }
        }

        cycleStartTime = currentTime;
    }

    lastMeasurement = digitalMeasurement;
}

#endif // !PidController_h