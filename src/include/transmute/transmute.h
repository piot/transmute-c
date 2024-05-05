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

typedef enum TransmuteParticipantInputType {
    TransmuteParticipantInputTypeNormal,
    TransmuteParticipantInputTypeNoInputInTime,
    TransmuteParticipantInputTypeWaitingForReJoin,
    TransmuteParticipantInputTypeJoined,
    TransmuteParticipantInputTypeLeft,
} TransmuteParticipantInputType;

typedef struct TransmuteParticipantInput {
    uint8_t participantId;
    uint8_t localPartyId; // Only valid when TransmuteParticipantInputTypeJoined
    TransmuteParticipantInputType inputType;
    const void* input;
    size_t octetSize;
} TransmuteParticipantInput;

typedef struct TransmuteInput {
    TransmuteParticipantInput* participantInputs;
    size_t participantCount;
} TransmuteInput;

int transmuteInputFindParticipantId(const TransmuteInput* input, uint8_t participantId);

typedef struct TransmuteVmVersion {
    uint16_t major;
    uint16_t minor;
    uint16_t patch;
} TransmuteVmVersion;

bool transmuteVmVersionIsEqual(const TransmuteVmVersion* a, const TransmuteVmVersion* b);

#endif
