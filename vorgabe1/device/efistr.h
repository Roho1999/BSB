/*****************************************************************************/
/* Betriebssysteme                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                         E F I _ S T R E A M                               */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Die Klasse CGA_Stream ermoeglicht die Ausgabe verschiedener Datentypen    */
/* als Zeichenketten auf dem CGA Bildschirm eines PCs.                       */
/* Fuer weitergehende Formatierung oder spezielle Effekte stehen die         */
/* Methoden der Klasse CGA_Screen zur Verfuegung.                            */
/*****************************************************************************/

#ifndef __efistr_include__
#define __efistr_include__

#include "object/o_stream.h"
#include "machine/efiscr.h"

class EFI_Stream : public O_Stream, public EFI_Screen
/* TODO: Hier muesst ihr selbst Code vervollstaendigen */ 
 {
public: 
      EFI_Stream() : O_Stream(), EFI_Screen() {};

      void flush();
private:
      EFI_Stream(EFI_Stream &copy) = delete; // Verhindere Kopieren


/* TODO: Hier muesst ihr selbst Code vervollstaendigen */

      

 };

#endif
