/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <transmute/transmute.h>

void transmuteInit(Transmute* self, TransmuteTickFn tickFn, size_t tickDurationMs)
{
    self->tickFn = tickFn;
    self->constantTickDurationMs = tickDurationMs;
}
