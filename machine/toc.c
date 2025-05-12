/*****************************************************************************/
/* Betriebssysteme                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                                 T O C                                     */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Die Struktur toc dient dazu, bei einem Koroutinenwechsel die Werte der    */
/* nicht-fluechtigen Register zu sichern.                                    */
/* Beim GNU C Compiler sind eax, ecx und edx fluechtige Register, die bei    */
/* Funktionsaufrufen und somit auch bei einem Koroutinenwechsel keine        */
/* spaeter noch benoetigten Werte haben duerfen. Daher muss in der Struktur  */
/* toc auch kein Platz fuer sie bereitgestellt werden.                       */
/*****************************************************************************/

#include "machine/toc.h"

// TOC_SETTLE: bereitet den Kontext der Koroutine fuer den ersten
//             Aufruf vor.
void toc_settle (struct toc* regs, void* tos, void (*kickoff)(void*),
		 void* object) {
  /* Notiz: Der msp430-Stackpointer zeigt hinter das erste freie Element */
  unsigned int *st = (unsigned int *)tos;

  /* Adresse von kickoff */
  *(--st) = (unsigned int)kickoff;

  /* Parameter fuer kickoff */
  *(--st) = (unsigned int)object;

  /* Ruecksprungadresse von toc_switch zu toc_pop_parameter */
  *(--st) = (unsigned int)toc_pop_parameter;

  for (int i=0; i<8; i++)
    /* Dummywerte fuer r4..r11 auf den neuen Stack legen */
    *(--st) = i;

  /* neuen Stackpointer speichern */
  regs->stackptr = st;
}
