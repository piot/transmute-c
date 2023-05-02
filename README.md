# Transmute

Can tick() a simulation with a given input, as well as get and set the simulation state.

## Usage

### Initialize

Setup function pointers for the simulation and the application version and tickDurationMs (normally around 16 ms).

```c
typedef struct TransmuteVmSetup {
    TransmuteTickFn tickFn;
    TransmuteGetStateFn getStateFn;
    TransmuteSetStateFn setStateFn;
    TransmuteStateToStringFn stateToString;
    TransmuteInputToStringFn inputToString;
    size_t tickDurationMs;
    TransmuteVmVersion version;
} TransmuteVmSetup;

void transmuteVmInit(TransmuteVm* self, void* vmPointer, TransmuteVmSetup setup, Clog log);
```

### Tick

Perform a simulation tick with the specified input.

```c
typedef struct TransmuteParticipantInput {
    uint8_t participantId;
    const void* input;
    size_t octetSize;
} TransmuteParticipantInput;

typedef struct TransmuteInput {
    TransmuteParticipantInput* participantInputs;
    size_t participantCount;
} TransmuteInput;

void transmuteVmTick(TransmuteVm* self, const TransmuteInput* input);
```

### Set and Get State

The simulation state is implementation dependant.

```c
typedef struct TransmuteState {
    const void* state;
    size_t octetSize;
} TransmuteState;

void transmuteVmSetState(TransmuteVm* self, const TransmuteState* state);
TransmuteState transmuteVmGetState(const TransmuteVm* self);
```
