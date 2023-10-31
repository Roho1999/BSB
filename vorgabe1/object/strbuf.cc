/*****************************************************************************/
/* Betriebssysteme                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                       S T R I N G B U F F E R                             */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Die Klasse Stringbuffer stellt einen Puffer fuer die Sammlung von Zeichen */
/* zur Darstellung auf dem Bildschirm oder anderen Ausgabegeraeten bereit.   */
/* Die Ausgabe der Zeichen erfolgt, sobald der Puffer voll ist oder wenn     */
/* explizit die Methode flush() aufgerufen wird.                             */
/* Da Stringbuffer geraeteunabhaengig sein soll, ist flush() eine virtuelle  */
/* Methode, die von den abgeleiteten Klassen definiert werden muss.          */
/*****************************************************************************/

#include "object/strbuf.h"

/* TODO: Hier muesst ihr selbst Code vervollstaendigen */ 


/**
 * @brief Diese Methode fügt das Zeichen c an das Ende der bereits gesammelten Zeichen an.
 *  Wenn die Pufferkapazität des Stringbuffer-Objektes nun erschöpft ist,
 *  müssen die gesammelten Zeichen durch Aufruf der Methode flush () verarbeitet werden    
 * 
 * @param c 
 */
void Stringbuffer::put (char c){
    this->charbuffer[this->position] = c;
    this->position += 1;
    if (this->position >= 5) 
    {
        this->flush();
        this->position = 0;
    }
}

//Die flush() Methode sollte denke nicht hier implementiert sein, die ist als true virtual deklariert 