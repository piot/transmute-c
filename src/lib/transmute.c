/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <clog/clog.h>
#include <transmute/transmute.h>
/// Checks that versions are equal
/// @param a version
/// @param b version
/// @return true if equal
bool transmuteVmVersionIsEqual(const TransmuteVmVersion* a, const TransmuteVmVersion* b)
{
    return a->major == b->major && a->minor == b->minor && a->patch == b->patch;
}

/// Finds the index given a participant id
/// @param input transmute input
/// @param participantId the participantId to search for
/// @return index or negative if failed
int transmuteInputFindParticipantId(const TransmuteInput* input, uint8_t participantId)
{
    for (size_t i=0; i< input->participantCount; ++i) {
        if (input->participantInputs[i].participantId == participantId) {
            return (int)i;
        }
    }

    return -1;
}
