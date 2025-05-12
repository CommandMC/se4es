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
/*                                                                           */
/* Achtung: Fuer den Zugriff auf die Elemente von struct toc aus einer       */
/* Assemblerfunktion heraus werden in der Datei toc.inc Namen fuer die       */
/* benoetigten Abstaende der einzelnen Elemente zum Anfang der Struktur      */
/* definiert. Damit dann auch auf die richtigen Elemente zugegriffen wird,   */
/* sollten sich die Angaben von toc.h und toc.inc exakt entsprechen.         */
/* Wer also toc.h aendert, muss auch toc.inc anpassen (und umgekehrt.)       */
/*****************************************************************************/

#ifndef __toc_include__
#define __toc_include__

#ifdef __cplusplus
extern "C" {
#endif

// TOC: ("thread of control")
//
struct toc
 {
     void *stackptr;
 };

void toc_settle (struct toc* regs, void* tos, void (*kickoff)(void*),
                 void* object);
  
void toc_go(struct toc* regs);
void toc_switch(struct toc* regs_now, struct toc* regs_then);
void toc_pop_parameter(void);

#ifdef __cplusplus
}
#endif

#endif

