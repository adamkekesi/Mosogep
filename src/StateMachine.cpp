#include "StateMachine.h"
#include <Arduino.h>

StateMachine::StateMachine(unsigned long disableTime10,
                           unsigned long disableTime11,
                           int disableFreq10,
                           int disableFreq11,
                           unsigned long cooldown6)
{
    this->disableTime10 = disableTime10;
    this->disableTime11 = disableTime11;
    this->cooldown6 = cooldown6;
    this->disableFreq10 = disableFreq10;
    this->disableFreq11 = disableFreq11;
    this->state6 = new Inactive6(this);
    this->state7 = new Inactive7(this);
}

bool StateMachine::Is7Overridden()
{
    return state6->GetName() == "Active";
}

int StateMachine::GetStateOf11()
{
    return state7->Is11Emitting() ? HIGH : LOW;
}

int StateMachine::GetStateOf10()
{
    return state7->Is10Emitting() ? HIGH : LOW;
}

void StateMachine::Tick(unsigned long timeSinceLastLoop, int input7, int input6, double frequency)
{
    state6->Tick(timeSinceLastLoop, input6, frequency);
    state7->Tick(timeSinceLastLoop, input7, frequency);
}

void StateMachine::ChangeState6(State6 *newState)
{
    State6 *previous = this->state6;
    this->state6 = newState;
    if (previous)
    {
        delete previous;
    }
}

void StateMachine::ChangeState7(State7 *newState)
{
    State7 *previous = this->state7;
    this->state7 = newState;
    if (previous)
    {
        delete previous;
    }
}

void Inactive6::SwitchToActive()
{
    stateMachine->ChangeState6(new Active6(stateMachine));
}

void Inactive6::Tick(unsigned long timeSinceLastLoop, int input6, double frequency)
{
    if (input6 == HIGH)
    {
        SwitchToActive();
    }
}

void Active6::SwitchToInactive()
{
    stateMachine->ChangeState6(new Inactive6(stateMachine));
}

void Active6::Tick(unsigned long timeSinceLastLoop, int input6, double frequency)
{
    timer += timeSinceLastLoop;
    if (timer >= stateMachine->cooldown6)
    {
        SwitchToInactive();
    }
}

void Inactive7::SwitchToActive()
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

void Inactive7::Tick(unsigned long timeSinceLastLoop, int input7, double frequency)
{
    if (input7 == HIGH)
    {
        SwitchToActive();
    }
}

void Active7::SwitchToInactive()
{
    stateMachine->ChangeState7(new Inactive7(stateMachine));
}

void Active7::Tick(unsigned long timeSinceLastLoop, int input7, double frequency)
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

void OverriddenActive7::SwitchToInactive()
{
    stateMachine->ChangeState7(new Inactive7(stateMachine));
}

void OverriddenActive7::Tick(unsigned long timeSinceLastLoop, int input7, double frequency)
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