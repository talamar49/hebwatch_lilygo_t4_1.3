#include <iostream>
#include <functional>

class StateMachine {
private:
    std::function<void()> currentState; // Pointer to the current state function

public:

    void ChangeState(std::function<void()> newState_);

    void Update();
};
