/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <clog/clog.h>
#include <transmute/transmute.h>

/// Initializes a transmute virtual machine
/// @param self
/// @param vmPointer
/// @param setup contains tick, getState and setState functions
void transmuteVmInit(TransmuteVm* self, void* vmPointer, TransmuteVmSetup setup, Clog log)
{
    self->tickFn = setup.tickFn;
    self->getStateFn = setup.getStateFn;
    self->setStateFn = setup.setStateFn;
    self->inputToString = setup.inputToString;
    self->stateToString = setup.stateToString;
    self->constantTickDurationMs = setup.tickDurationMs;
    self->vmPointer = vmPointer;
    self->log = log;
    self->version = setup.version;
    self->initialStateIsSet = false;
}

/// Sets a state to the virtual machine
/// Replaces the internal cached state with the specified state.
/// @param self
/// @param state
void transmuteVmSetState(TransmuteVm* self, const TransmuteState* state)
{
    self->setStateFn(self->vmPointer, state);
    self->initialStateIsSet = true;
}

/// Ticks the virtual machine. The virtual machine keeps the mutated state in an efficient way
/// that cache must not affect the determinism
/// @param self
/// @param input
void transmuteVmTick(TransmuteVm* self, const TransmuteInput* input)
{
    if (!self->initialStateIsSet) {
        CLOG_ERROR("can not tick, since initial state has not been set")
    }
#if TRANSMUTE_LOG_TICK
    CLOG_C_VERBOSE(&self->log, "tick with inputs:")
    char inputDebugString[64];
    for (size_t i=0; i<input->participantCount; ++i) {
        self->inputToString(self->vmPointer, &input->participantInputs[i], inputDebugString, 64);
        CLOG_C_VERBOSE(&self->log, "   input: index:%zu participant:%02X '%s'", i, input->participantInputs[i].participantId, inputDebugString)
    }
#endif

    self->tickFn(self->vmPointer, input);
}

/// Gets the transmute state from the virtual machine internal representation.
/// @param self
/// @return
TransmuteState transmuteVmGetState(const TransmuteVm* self)
{
    if (!self->initialStateIsSet) {
        CLOG_ERROR("can not read state, no state has been set")
    }
    return self->getStateFn(self->vmPointer);
}

/// Convert a TransmuteState to a string
/// @param self
/// @param state
/// @param target
/// @param maxTargetOctetSize
/// @return
int transmuteVmStateToString(const TransmuteVm* self, const TransmuteState* state, char* target, size_t maxTargetOctetSize)
{
    return self->stateToString(self->vmPointer, state, target, maxTargetOctetSize);
}

/// Convert a Transmute
/// @param self
/// @param input
/// @param target
/// @param maxTargetOctetSize
/// @return
int transmuteVmInputToString(const TransmuteVm* self, const TransmuteParticipantInput* input, char* target, size_t maxTargetOctetSize)
{
    return self->inputToString(self->vmPointer, input, target, maxTargetOctetSize);
}

/// Checks if a state has been set to the virtual machine
/// @param self
/// @return
bool transmuteVmHasState(const TransmuteVm* self)
{
    return self->initialStateIsSet;
}

/// Checks that versions are equal
/// @param a
/// @param b
/// @return
bool transmuteVmVersionIsEqual(const TransmuteVmVersion* a, const TransmuteVmVersion* b)
{
    return a->major == b->major && a->minor == b->minor && a->patch == b->patch;
}


int transmuteInputFindParticipantId(const TransmuteInput* input, uint8_t participantId)
{
    for (size_t i=0; i< input->participantCount; ++i) {
        if (input->participantInputs[i].participantId == participantId) {
            return i;
        }
    }

    return -1;
}
