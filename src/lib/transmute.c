/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <transmute/transmute.h>

/// Initializes a transmute virtual machine
/// @param self
/// @param vmPointer
/// @param setup contains tick, getState and setState functions
void transmuteVmInit(TransmuteVm* self, void* vmPointer, TransmuteVmSetup setup)
{
    self->tickFn = setup.tickFn;
    self->getStateFn = setup.getStateFn;
    self->setStateFn = setup.setStateFn;
    self->constantTickDurationMs = setup.tickDurationMs;
    self->vmPointer = vmPointer;
}

/// Sets a state to the virtual machine
/// Replaces the internal cached state with the specified state.
/// @param self
/// @param state
void transmuteVmSetState(TransmuteVm* self, const TransmuteState* state)
{
    self->setStateFn(self->vmPointer, state);
}

/// Ticks the virtual machine. The virtual machine keeps the mutated state in an efficient way
/// that cache must not affect the determinism
/// @param self
/// @param input
void transmuteVmTick(TransmuteVm* self, const TransmuteInput* input)
{
    self->tickFn(self->vmPointer, input);
}

/// Gets the transmute state from the virtual machine internal representation.
/// @param self
/// @return
TransmuteState transmuteVmGetState(const TransmuteVm* self)
{
    return self->getStateFn(self->vmPointer);
}
