#include <Arduino.h>
#if !defined(StateMachine_h)
#define StateMachine_h

class State6;
class Inactive6;
class Active6;
class State7;
class Inactive7;
class Active7;
class OverriddenActive7;

using namespace std;
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

    double disableFreq10;
    double disableFreq11;

    StateMachine(unsigned long disableTime10,
                 unsigned long disableTime11,
                 double disableFreq10,
                 double disableFreq11,
                 unsigned long cooldown6);

    ~StateMachine()
    {
    }

    bool Is7Overridden();

    int GetStateOf11();

    int GetStateOf10();

    void Tick(unsigned long timeSinceLastLoop, int input7, int input6, double frequency);

    void ChangeState6(State6 *newState);

    void ChangeState7(State7 *newState);
};

class State6
{
protected:
    StateMachine *stateMachine;
    
    State6(StateMachine *stateMachine)
    {
        this->stateMachine = stateMachine;
    }

public:
    virtual ~State6()
    {
    }

    virtual void Tick(unsigned long timeSinceLastLoop, int input6, double frequency)
    {
    }

    virtual String GetName()
    {
        return "";
    }
};

class Inactive6 : public State6
{
private:
    void SwitchToActive();

public:
    Inactive6(StateMachine *sm) : State6(sm)
    {
    }

    ~Inactive6()
    {
    }

    void Tick(unsigned long timeSinceLastLoop, int input6, double frequency);

    String GetName()
    {
        return "Inactive";
    }
};

class Active6 : public State6
{
private:
    unsigned long timer = 0;

    void SwitchToInactive();

public:
    Active6(StateMachine *sm) : State6(sm)
    {
    }

    ~Active6()
    {
    }

    void Tick(unsigned long timeSinceLastLoop, int input6, double frequency);

    String GetName()
    {
        return "Active";
    }
};

class State7
{
protected:
    StateMachine *stateMachine;

    State7(StateMachine *stateMachine)
    {
        this->stateMachine = stateMachine;
    }

public:
    virtual ~State7()
    {
    }

    virtual void Tick(unsigned long timeSinceLastLoop, int input7, double frequency)
    {
    }

    virtual String GetName()
    {
        return "";
    }

    virtual bool Is10Emitting()
    {
        return false;
    }

    virtual bool Is11Emitting()
    {
        return false;
    }
};

class Inactive7 : public State7
{
private:
    void SwitchToActive();

public:
    Inactive7(StateMachine *sm) : State7(sm)
    {
    }

    ~Inactive7()
    {
    }

    void Tick(unsigned long timeSinceLastLoop, int input7, double frequency);

    String GetName()
    {
        return "Inactive";
    }
};

class Active7 : public State7
{
private:
    unsigned long timer = 0;

    void SwitchToInactive();

public:
    Active7(StateMachine *sm) : State7(sm)
    {
    }

    ~Active7()
    {
    }

    void Tick(unsigned long timeSinceLastLoop, int input7, double frequency);

    String GetName()
    {
        return "Active";
    }

    bool Is10Emitting()
    {
        return false;
    }

    bool Is11Emitting()
    {
        return true;
    }
};

class OverriddenActive7 : public State7
{
private:
    unsigned long timer = 0;

    void SwitchToInactive();

public:
    OverriddenActive7(StateMachine *sm) : State7(sm)
    {
    }

    ~OverriddenActive7()
    {
    }

    void Tick(unsigned long timeSinceLastLoop, int input7, double frequency);

    String GetName()
    {
        return "OverriddenActive";
    }

    bool Is10Emitting()
    {
        return true;
    }

    bool Is11Emitting()
    {
        return false;
    }
};

#endif // StateMachine_h
