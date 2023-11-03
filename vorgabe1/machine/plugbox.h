/*****************************************************************************/
/* Betriebssysteme                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              P L U G B O X                                */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Abstraktion einer Interruptvektortabelle. Damit kann man die Adresse der  */
/* Behandlungsroutine fuer jeden Hardware-, Softwareinterrupt und jede       */
/* Prozessorexception festlegen.                                             */
/*****************************************************************************/

#ifndef __Plugbox_include__
#define __Plugbox_include__

#include "guard/gate.h"

class Plugbox
 {
private:
    Plugbox(const Plugbox &other) = delete; // Verhindere Kopieren
    Plugbox& operator=(const Plugbox& other) = delete; // Verhindere Kopieren

/* TODO: Hier muesst ihr selbst Code vervollstaendigen */ 
    void assign (unsigned int slot, Gate& gate);

    Gate& report (unsigned int slot)
 };

#endif
