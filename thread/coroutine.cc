/*****************************************************************************/
/* Betriebssysteme                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                          C O R O U T I N E                                */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Implementierung des Koroutinenkonzepts.                                   */
/* Im Konstruktor wird der initiale Kontext der Koroutine eingerichtet.      */
/* Mit go() wird die erste Koroutine aktiviert. Alle weiteren Koroutinen-    */
/* wechsel sollten mit resume() erfolgen.                                    */
/* Um bei einem Koroutinenwechsel den Kontext sichern zu koennen, enthaelt   */
/* jedes Koroutinenobjekt eine Struktur toc, in dem die Werte der nicht-     */
/* fluechtigen Register gesichert werden koennen.                            */
/*****************************************************************************/

#include "device/panic.h"
#include "machine/toc.h"
#include "thread/coroutine.h"

// Funktionen, die auf der Assembler-Ebene implementiert werden, muessen als
// extern "C" deklariert werden, da sie nicht dem Name-Mangeling von C++ 
// entsprechen.
extern "C" {
  // Siehe toc.h
  void kickoff(void *foo);
}

Coroutine::Coroutine(void *tos) {
  toc_settle(&myregs, tos, &kickoff, this);
}

void Coroutine::go() {
  toc_go(&myregs);
}

void Coroutine::resume(Coroutine &next) {
  toc_switch(&myregs, &next.myregs);
}
