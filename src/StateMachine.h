#if !defined(StateMachine_h)
#define StateMachine_h
#include "States6.h"
#include "States7.h"

class StateMachine
{
private:
    /* data */
public:
    State6 *state6;
    State7 *state7;

    unsigned long disableTime10;
    unsigned long disableTime11;
    unsigned long cooldown6;

    int disableFreq10;
    int disableFreq11;

    StateMachine(unsigned long disableTime10,
                 unsigned long disableTime11,
                 int disableFreq10,
                 int disableFreq11,
                 unsigned long cooldown6)
    {
        state6 = new Inactive6(this);
        state7 = new Inactive7(this);
        this->disableTime10 = disableTime10;
        this->disableTime11 = disableTime11;
        this->cooldown6 = cooldown6;
        this->disableFreq10 = disableFreq10;
        this->disableFreq11 = disableFreq11;
    }

    ~StateMachine();

    bool Is7Overridden()
    {
        return state6->name == "Active";
    }

    int GetStateOf11(){
        return state7->is11Emitting ? HIGH : LOW;
    }

    int GetStateOf10(){
        return state7->is10Emitting ? HIGH : LOW;
    }

    void Tick(unsigned long timeSinceLastLoop, int input7, int input6, double frequency)
    {
        state6->Tick(timeSinceLastLoop, input6, frequency);
        state7->Tick(timeSinceLastLoop, input7, frequency);
    }

    void ChangeState6(State6 *newState)
    {
        if (state6)
        {
            delete state6;
        }
        state6 = newState;
    }

    void ChangeState7(State7 *newState)
    {
        if (state7)
        {
            delete state7;
        }
        state7 = newState;
    }
};

#endif // StateMachine_h
