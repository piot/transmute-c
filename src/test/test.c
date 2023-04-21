/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include "clog/clog.h"
#include "transmute/transmute.h"
#include "utest.h"

typedef struct AppSpecificState {
    int x;
    int time;
} AppSpecificState;

typedef struct AppSpecificParticipantInput {
    int horizontalAxis;
} AppSpecificParticipantInput;

typedef struct AppSpecificVm {
    AppSpecificState appSpecificState;
} AppSpecificVm;

void appSpecificTick(void* _self, const TransmuteInput* input)
{
    AppSpecificVm* self = (AppSpecificVm*) _self;

    if (input->participantCount > 0) {
        const AppSpecificParticipantInput* appSpecificInput = (AppSpecificParticipantInput*) input->participantInputs[0]
                                                                  .input;
        if (appSpecificInput->horizontalAxis > 0) {
            self->appSpecificState.x++;
            CLOG_DEBUG("app: tick with input %d, walking to the right", appSpecificInput->horizontalAxis)
        } else {
            CLOG_DEBUG("app: tick with input %d, not walking to the right", appSpecificInput->horizontalAxis)
        }
    } else {
        CLOG_DEBUG("app: tick with no input")
    }

    self->appSpecificState.time++;
}

TransmuteState appSpecificGetState(const void* _self)
{
    TransmuteState state;

    AppSpecificVm* self = (AppSpecificVm*) _self;

    state.octetSize = sizeof(AppSpecificState);
    state.state = (const void*) &self->appSpecificState;

    return state;
}

void appSpecificSetState(void* _self, const TransmuteState* state)
{
    AppSpecificVm* self = (AppSpecificVm*) _self;

    self->appSpecificState = *((const AppSpecificState*) state->state);
}

int appSpecificStateToString(void* _self, const TransmuteState* state, char* target, size_t maxTargetOctetSize)
{
    (void) _self;

    const AppSpecificState* appState = (AppSpecificState*) state->state;
    return tc_snprintf(target, maxTargetOctetSize, "state: time: %d pos.x: %d", appState->time, appState->x);
}

int appSpecificInputToString(void* _self, const TransmuteParticipantInput* input, char* target,
                             size_t maxTargetOctetSize)
{
    (void) _self;
    const AppSpecificParticipantInput* participantInput = (AppSpecificParticipantInput*) input->input;
    return tc_snprintf(target, maxTargetOctetSize, "input: horizontalAxis: %d", participantInput->horizontalAxis);
}

UTEST(Transmute, tick)
{
    TransmuteVm transmuteVm;
    AppSpecificVm appVm;

    TransmuteVmSetup setup;
    setup.getStateFn = appSpecificGetState;
    setup.setStateFn = appSpecificSetState;
    setup.tickFn = appSpecificTick;
    setup.stateToString = appSpecificStateToString;
    setup.inputToString = appSpecificInputToString;
    setup.tickDurationMs = 16;

    Clog subLog;
    subLog.config = &g_clog;
    subLog.constantPrefix = "AuthoritativeVm";
    transmuteVmInit(&transmuteVm, &appVm, setup, subLog);

    AppSpecificState initialAppState;
    initialAppState.time = 0;
    initialAppState.x = 0;

    TransmuteState initialTransmuteState;
    initialTransmuteState.octetSize = sizeof(initialAppState);
    initialTransmuteState.state = (void*) &initialAppState;

    transmuteVmSetState(&transmuteVm, &initialTransmuteState);

    ASSERT_EQ(0, appVm.appSpecificState.x);
    ASSERT_EQ(0, appVm.appSpecificState.time);

    AppSpecificParticipantInput appSpecificInput;
    appSpecificInput.horizontalAxis = 24;

    TransmuteInput transmuteInput;
    TransmuteParticipantInput participantInput;

    participantInput.input = &appSpecificInput;
    participantInput.octetSize = sizeof(appSpecificInput);

    transmuteInput.participantInputs = &participantInput;
    transmuteInput.participantCount = 1;

    transmuteVmTick(&transmuteVm, &transmuteInput);

    ASSERT_EQ(1, appVm.appSpecificState.x);
    ASSERT_EQ(1, appVm.appSpecificState.time);

    appSpecificInput.horizontalAxis = -42;

    transmuteVmTick(&transmuteVm, &transmuteInput);

    ASSERT_EQ(1, appVm.appSpecificState.x);
    ASSERT_EQ(2, appVm.appSpecificState.time);

    TransmuteState state = transmuteVmGetState(&transmuteVm);
    ASSERT_EQ(sizeof(AppSpecificState), state.octetSize);
    ASSERT_EQ(0, tc_memcmp(state.state, &appVm.appSpecificState, state.octetSize));

    AppSpecificState wrongState;
    wrongState.time = 99;
    wrongState.x = 0;
    ASSERT_NE(0, tc_memcmp(state.state, &wrongState, state.octetSize));
}
