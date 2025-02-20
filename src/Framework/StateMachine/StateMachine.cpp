#include "Framework/StateMachine/StateMachine.h"


void StateMachine::ChangeState(std::function<void()> newState_) 
{
    currentState = newState_;
}

void StateMachine::Update() 
{
    if (currentState) {
        currentState(); // Call the current state function
    }
}