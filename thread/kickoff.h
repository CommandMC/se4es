#ifndef KICKOFF_H
#define KICKOFF_H

#include "thread/coroutine.h"

#ifdef __cplusplus
extern "C" {
#endif

void kickoff(Coroutine *object);

#ifdef __cplusplus
}
#endif

#endif
