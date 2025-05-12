#include "device/panic.h"

#ifndef __acweaving // no weaving in this file

void operator delete(void *ptr) throw() {}
void operator delete(void*, unsigned int) throw() {}

extern "C" void __cxa_pure_virtual(void) {
  Panic::instance().panic(PANIC_PUREVIRTUAL);
}

extern "C" void atexit(void (*arg1)(void*), void* arg2, void* arg3) {}

#endif
