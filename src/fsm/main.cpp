// Simple FSM demo - reads input from stdin and outputs FSM events
// Use this to verify your implementation works before running tests
//
// Usage: ./fsm < input.txt
// Or interactively: ./fsm (then type commands)

#include "fsm.h"
#include <iostream>
#include <string>

int main()
{
    // Create the pre-configured agent FSM
    auto fsm = FSM::createAgentFSM();

    // Read initial state from first line
    std::string initialState;
    if (!(std::cin >> initialState))
    {
        std::cerr << "Error: Expected initial state (IDLE, ALERT, or COMBAT)" << std::endl;
        return 1;
    }

    // Set the initial state (triggers ENTER and EXECUTE)
    fsm.setInitialState(initialState, std::cout);

    // Process commands until EOF
    std::string command;
    while (std::cin >> command)
    {
        fsm.processCommand(command, std::cout);
    }

    return 0;
}
