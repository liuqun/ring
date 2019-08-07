#pragma once

#include "toolbox/ring.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct ring_iterator_st RingIterator;

RingIterator * ring_iterator_new(const Ring *ring);

void * ring_iterator_peek(RingIterator *iter);
void ring_iterator_step_forward(RingIterator *iter);
int ring_iterator_step_is_end(RingIterator *iter);

void ring_iterator_free(RingIterator *iter);

#ifdef __cplusplus
}; /* extern "C" */
#endif /* __cplusplus */
