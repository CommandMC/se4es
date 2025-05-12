#ifndef SIMPLICITY_H
#define SIMPLICITY_H

#ifdef __cplusplus
extern "C" {
#endif

#include "machine/simpliciti/bsp.h"
#include "machine/simpliciti/mrfi/mrfi.h"
#include "machine/simpliciti/nwk/nwk_types.h"
#include "machine/simpliciti/nwk/nwk.h"
#include "machine/simpliciti/nwk/nwk_api.h"

void MRFI_RadioIsr(void);

#ifdef __cplusplus
}
#endif

#endif
