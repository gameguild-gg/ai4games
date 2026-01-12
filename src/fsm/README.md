# Finite State Machine (FSM)

You are a game developer tasked with implementing a **Finite State Machine** framework that follows best practices. Your FSM should cleanly separate **states** from **transitions** and avoid the naive `switch/if-else` approach.

## Learning Objectives

- Understand the State Pattern and its advantages over naive implementations
- Implement a clean separation between states and transition logic
- Design reusable and testable FSM components
- Practice encapsulation and polymorphism in C++

## Requirements

Your FSM implementation must:

1. **Separate States and Transitions**: States should not contain transition logic. Transitions should be registered separately with condition functions.
2. **Support State Lifecycle**: Each state must have `onEnter()`, `execute()`, and `onExit()` methods.
3. **Be Data-Driven**: Transitions should be configurable at runtime, not hardcoded.
4. **Be Testable**: The FSM should be easily testable through input/output verification.

## The Scenario: Simple AI Agent

Implement an FSM for a simple AI agent with the following states:

```
┌─────────────────────────────────────────────────────────────┐
│                                                             │
│    ┌─────────┐   "alert"    ┌─────────┐   "engage"          │
│    │  IDLE   │ ──────────►  │  ALERT  │ ──────────►         │
│    └─────────┘              └─────────┘             │       │
│         ▲                        │                  │       │
│         │                        │ "calm"           ▼       │
│         │    "rest"         ┌────┴────┐        ┌────────┐   │
│         └───────────────────│  IDLE   │◄───────│ COMBAT │   │
│                             └─────────┘ "idle" └────────┘   │
│                                                             │
└─────────────────────────────────────────────────────────────┘
```

### States

| State    | Description                             |
| -------- | --------------------------------------- |
| `IDLE`   | Agent is resting, not doing anything    |
| `ALERT`  | Agent detected something, investigating |
| `COMBAT` | Agent is engaged in combat              |

### Transitions

| From   | To     | Trigger Command |
| ------ | ------ | --------------- |
| IDLE   | ALERT  | `alert`         |
| ALERT  | COMBAT | `engage`        |
| ALERT  | IDLE   | `calm`          |
| COMBAT | IDLE   | `idle`          |

## Input Format

The input consists of:

1. First line: Initial state name (one of: `IDLE`, `ALERT`, `COMBAT`)
2. Following lines: Commands to process, one per line
3. Input ends at EOF

```
IDLE
alert
engage
idle
```

## Output Format

For each state transition and execution, output:

1. When entering a state: `ENTER:<STATE_NAME>`
2. When executing a state: `EXECUTE:<STATE_NAME>`
3. When exiting a state: `EXIT:<STATE_NAME>`
4. If a command doesn't trigger a valid transition: `INVALID:<COMMAND>`

**Important**: The initial state should trigger `ENTER` and `EXECUTE`. Each command triggers a transition check, and if valid, the full cycle: `EXIT` (old) → `ENTER` (new) → `EXECUTE` (new).

### Example

**Input:**

```
IDLE
alert
engage
idle
```

**Output:**

```
ENTER:IDLE
EXECUTE:IDLE
EXIT:IDLE
ENTER:ALERT
EXECUTE:ALERT
EXIT:ALERT
ENTER:COMBAT
EXECUTE:COMBAT
EXIT:COMBAT
ENTER:IDLE
EXECUTE:IDLE
```

### Example with Invalid Command

**Input:**

```
IDLE
engage
alert
```

**Output:**

```
ENTER:IDLE
EXECUTE:IDLE
INVALID:engage
EXIT:IDLE
ENTER:ALERT
EXECUTE:ALERT
```

## Implementation Hints

Your `fsm.h` should include:

1. **A State base class** with virtual lifecycle methods
2. **Concrete state classes** (IdleState, AlertState, CombatState)
3. **An FSM class** that:
   - Manages registered states
   - Stores transitions as a mapping of (current_state, trigger) → next_state
   - Processes commands and triggers appropriate transitions

### Architecture Suggestion

```cpp
// Pseudocode structure - you need to implement this properly!

class State {
    // Virtual methods for lifecycle
    // A way to identify the state (name/enum)
};

class FSM {
    // Current state pointer
    // Map of states by name
    // Map of transitions: (from_state, trigger) -> to_state

    // Methods to:
    // - Register states
    // - Add transitions
    // - Process commands
    // - Change states (handling exit/enter/execute)
};
```

## What NOT to Do (Naive Approach)

```cpp
// DON'T DO THIS - Hardcoded transitions in switch/if-else
void update(string command) {
    switch(currentState) {
        case IDLE:
            if(command == "alert") currentState = ALERT;
            break;
        case ALERT:
            if(command == "engage") currentState = COMBAT;
            else if(command == "calm") currentState = IDLE;
            break;
        // ... this doesn't scale!
    }
}
```

## Grading Criteria

- **Correctness** (40%): All test cases pass
- **Architecture** (30%): Clean separation of states and transitions
- **Code Quality** (20%): Proper use of OOP principles, readable code
- **Edge Cases** (10%): Handles invalid commands gracefully

## Files to Modify

- `fsm.h` - Your FSM implementation (header-only)

## Testing

Run the tests with:

```bash
cd build
make fsm-tests
./out/fsm/fsm-tests
```

Or test manually:

```bash
./out/fsm/fsm < tests/simple_transitions.in
```
