/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef TRANSMUTE_H
#define TRANSMUTE_H

#include <clog/clog.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct TransmuteState {
    const void* state;
    size_t octetSize;
} TransmuteState;

typedef struct TransmuteParticipantInput {
    const void* input;
    size_t octetSize;
} TransmuteParticipantInput;

typedef struct TransmuteInput {
    TransmuteParticipantInput* participantInputs;
    size_t participantCount;
} TransmuteInput;

typedef void (*TransmuteTickFn)(void* vmPointer, const TransmuteInput* input);
typedef TransmuteState (*TransmuteGetStateFn)(const void* vmPointer);
typedef void (*TransmuteSetStateFn)(void* vmPointer, const TransmuteState* state);

typedef int (*TransmuteStateToStringFn)(void* vmPointer, const TransmuteState* state, char* target, size_t maxSize);
typedef int (*TransmuteInputToStringFn)(void* vmPointer, const TransmuteParticipantInput* input, char* target,
                                        size_t maxSize);

typedef struct TransmuteVm {
    TransmuteTickFn tickFn;
    TransmuteGetStateFn getStateFn;
    TransmuteSetStateFn setStateFn;
    TransmuteStateToStringFn stateToString;
    TransmuteInputToStringFn inputToString;
    size_t constantTickDurationMs;
    void* vmPointer;
    Clog log;
} TransmuteVm;

typedef struct TransmuteVmSetup {
    TransmuteTickFn tickFn;
    TransmuteGetStateFn getStateFn;
    TransmuteSetStateFn setStateFn;
    TransmuteStateToStringFn stateToString;
    TransmuteInputToStringFn inputToString;
    size_t tickDurationMs;

} TransmuteVmSetup;

void transmuteVmInit(TransmuteVm* self, void* vmPointer, TransmuteVmSetup setup, Clog log);
void transmuteVmSetState(TransmuteVm* self, const TransmuteState* state);
void transmuteVmTick(TransmuteVm* self, const TransmuteInput* input);
TransmuteState transmuteVmGetState(const TransmuteVm* self);
int transmuteVmStateToString(const TransmuteVm* self, const TransmuteState* state, char* target,
                             size_t maxTargetOctetSize);
int transmuteVmInputToString(const TransmuteVm* self, const TransmuteParticipantInput* input, char* target,
                             size_t maxTargetOctetSize);

#endif
