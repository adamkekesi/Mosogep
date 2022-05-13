#if !defined(States7_h)
#define States7_h
#include "StateMachine.h"
#include <Arduino.h>

using namespace std;

class State7
{
protected:
    StateMachine *stateMachine;

    State7(StateMachine *stateMachine)
    {
        stateMachine = stateMachine;
    }

public:
    String name = "";

    bool is10Emitting;
    bool is11Emitting;

    ~State7()
    {
    }

    virtual void Tick(unsigned long timeSinceLastLoop, int input7, double frequency)
    {
    }
};

class Inactive7 : public State7
{
private:
    void SwitchToActive()
    {
        if (stateMachine->Is7Overridden())
        {
            stateMachine->ChangeState7(new OverriddenActive7(stateMachine));
        }
        else
        {
            stateMachine->ChangeState7(new Active7(stateMachine));
        }
    }

public:
    String name = "Inactive";

    bool is10Emitting = false;
    bool is11Emitting = false;

    Inactive7(StateMachine *sm) : State7(sm)
    {
    }

    ~Inactive7()
    {
    }

    void Tick(unsigned long timeSinceLastLoop, int input7, double frequency)
    {
        if (input7 == HIGH)
        {
            SwitchToActive();
        }
    }
};

class Active7 : public State7
{
private:
    unsigned long timer = 0;

    void SwitchToInactive()
    {
        stateMachine->ChangeState7(new Inactive7(stateMachine));
    }

public:
    String name = "Active";

    bool is10Emitting = false;
    bool is11Emitting = true;

    Active7(StateMachine *sm) : State7(sm)
    {
    }

    ~Active7()
    {
    }

    void Tick(unsigned long timeSinceLastLoop, int input7, double frequency)
    {
        timer += timeSinceLastLoop;
        if (timer >= stateMachine->disableTime11)
        {
            SwitchToInactive();
            return;
        }
        if (frequency <= stateMachine->disableFreq11)
        {
            SwitchToInactive();
            return;
        }
    }
};

class OverriddenActive7 : public State7
{
private:
    unsigned long timer = 0;

    void SwitchToInactive()
    {
        stateMachine->ChangeState7(new Inactive7(stateMachine));
    }

public:
    String name = "OverriddenActive";

    bool is10Emitting = true;
    bool is11Emitting = false;

    OverriddenActive7(StateMachine *sm) : State7(sm)
    {
    }

    ~OverriddenActive7()
    {
    }

    void Tick(unsigned long timeSinceLastLoop, int input7, double frequency)
    {
        timer += timeSinceLastLoop;
        if (timer >= stateMachine->disableTime10)
        {
            SwitchToInactive();
            return;
        }
        if (frequency <= stateMachine->disableFreq10)
        {
            SwitchToInactive();
            return;
        }
    }
};

#endif // States7_h
