/*****************************************************************************/
/* Betriebssysteme                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                         E F I _ S T R E A M                               */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Die Klasse EFI_Stream ermoeglicht die Ausgabe verschiedener Datentypen    */
/* als Zeichenketten auf dem CGA Bildschirm eines PCs.                       */
/* Fuer weitergehende Formatierung oder spezielle Effekte stehen die         */
/* Methoden der Klasse EFI_Screen zur Verfuegung.                            */
/*****************************************************************************/

#include "device/efistr.h"

/* TODO: Hier muesst ihr selbst Code vervollstaendigen */ 


void EFI_Stream::flush()
{
    print(this->charbuffer, Pixel(0xffffff), Pixel(0x000000));
    for (unsigned int i = 0; charbuffer[i] != '\0'; i++)
    {
        this->charbuffer[i] = '\0';
        
    }
    this->position = 0;
}


