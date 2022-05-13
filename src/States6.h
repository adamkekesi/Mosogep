#if !defined(States6_h)
#define States6_h
#include "StateMachine.h"
#include <Arduino.h>

using namespace std;

class State6
{
protected:
    StateMachine *stateMachine;

    State6(StateMachine *stateMachine)
    {
        stateMachine = stateMachine;
    }
    
public:
    String name = "";


    ~State6()
    {
    }

    virtual void Tick(unsigned long timeSinceLastLoop, int input6, double frequency)
    {

    }
};

class Inactive6 : public State6
{
private:
    void SwitchToActive(){
        stateMachine->ChangeState6(new Active6(stateMachine));
    }

public:
    String name = "Inactive";

    Inactive6(StateMachine *sm) : State6(sm)
    {
    }

    ~Inactive6()
    {
    }

    void Tick(unsigned long timeSinceLastLoop, int input6, double frequency){
        if (input6 == HIGH)
        {
            SwitchToActive();
        }
        
    }
};

class Active6 : public State6
{
private:
    unsigned long timer = 0;

    void SwitchToInactive()
    {
        stateMachine->ChangeState6(new Inactive6(stateMachine));
    }

public:
    String name = "Active";

    Active6(StateMachine *sm) : State6(sm)
    {
    }

    ~Active6()
    {
    }

    void Tick(unsigned long timeSinceLastLoop, int input6, double frequency)
    {
        timer += timeSinceLastLoop;
        if (timer >= stateMachine->cooldown6)
        {
            SwitchToInactive();
        }
    }
};

#endif // States6_h
