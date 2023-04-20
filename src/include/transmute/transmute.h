/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#ifndef TRANSMUTE_H
#define TRANSMUTE_H

#include <stdbool.h>
#include <stddef.h>

typedef struct TransmuteState {
    const void* state;
    size_t octetSize;
} TransmuteState;

typedef struct TransmutePartipiantInput {
    const void* input;
    size_t octetSize;
} TransmutePartipiantInput;

typedef struct TransmuteInput {
    TransmutePartipiantInput* participantInputs;
    size_t participantCount;
} TransmuteInput;

typedef TransmuteState (*TransmuteTickFn)(const TransmuteState* state, const TransmuteInput* input);

typedef struct Transmute {
    TransmuteTickFn tickFn;
    size_t constantTickDurationMs;
} Transmute;

void transmuteInit(Transmute* self, TransmuteTickFn tickFn, size_t tickDurationMs);

#endif
