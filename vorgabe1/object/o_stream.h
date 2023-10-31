/*****************************************************************************/
/* Betriebssysteme                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                            O _ S T R E A M                                */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Die Klasse O_Stream enthaelt die Definition des << Operators fuer die     */
/* wichtigsten der vordefinierten Datentypen und realisiert somit die        */
/* bekannte Ausgabefunktion der C++ iO_Stream Bibliothek. Zur Zeit wird die  */
/* Darstellung von Zeichen, Zeichenketten und ganzen Zahlen unterstuetzt.    */
/* Ein weiterer << Operator erlaubt die Verwendung von Manipulatoren.        */
/*                                                                           */
/* Neben der Klasse O_Stream sind hier auch die Manipulatoren hex, dec, oct  */
/* und bin fuer die Wahl der Basis bei der Zahlendarstellung, sowie endl     */
/* fuer den Zeilenumbruch definiert.                                         */
/*****************************************************************************/

#ifndef __o_stream_include__
#define __o_stream_include__

#include "object/strbuf.h"

class O_Stream: Stringbuffer
 {
public:
      /**
       * @brief Anfügen des Zeichens c an die gesammelten Zeichen.
       * 
       */
      O_Stream& operator<< (unsigned char c);
      O_Stream& operator<< (char c);

      /**
       * @brief Anfügen der Zahl number in dem gewählten Zahlensystem.
       * 
       */
      O_Stream& operator<< (unsigned short number);
      O_Stream& operator<< (short number);
      O_Stream& operator<< (unsigned int number);
      O_Stream& operator<< (int number);
      O_Stream& operator<< (unsigned long number);
      O_Stream& operator<< (long number);
      /**
       * @brief Anfügen des Zeigerwertes pointer im Hexadezimalsystem.

       * 
       */
      O_Stream& operator<< (void* pointer);

      /**
       * @brief Anfügen der nullterminierten Zeichenkette text (ohne die Nullterminierung).
       * 
       */
      
      O_Stream& operator<< (char* text);



      /**
       * @brief  Aufruf der Manipulatorfunktion fkt
       * 
       * @param fkt 
       * @return O_Stream& 
       */
      O_Stream& operator<< (O_Stream& (*fkt) (O_Stream&));

      //Es müsste eigentlich auch noch die flush-Methode aus der Stringbuffer Klasse überschrieben werden,
      //habs hier noch nicht reingeschrieben da das nicht in der Aufgabenstellung steht.


private:
      O_Stream(const O_Stream &copy); // Verhindere Kopieren
/* TODO: Hier muesst ihr selbst Code vervollstaendigen */     
 };

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                          M A N I P U L A T O R E N                        */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Die folgenden Funktionen erhalten und liefern jeweils eine Referenz auf   */
/* ein O_Stream Objekt. Da die Klasse O_Stream einen Operator << fuer        */
/* derartige Funktionen definiert, koennen sie mit Hilfe dieses Operators    */
/* aufgerufen und sogar in weitere Eingaben eingebettet werden.              */
/* Aufgabe der Manipulatoren ist, die Darstellung der nachfolgenden Ausgaben */
/* zu beeinflussen, z.B durch die Wahl des Zahlensystems.                    */
/*---------------------------------------------------------------------------*/

/**
 * @brief fügt einen Zeilenumbruch ein.
 * 
 * @param os 
 * @return O_Stream& 
 */
O_Stream& endl (O_Stream& os);

/**
 * @brief wählt das binäre Zahlensystem aus.
 * 
 * @param os 
 * @return O_Stream& 
 */
O_Stream& bin (O_Stream& os);

/**
 * @brief wählt das oktale Zahlensystem aus.
 * 
 * @param os 
 * @return O_Stream& 
 */
O_Stream& oct (O_Stream& os);

/**
 * @brief wählt das dezimale Zahlensystem aus.
 * 
 * @param os 
 * @return O_Stream& 
 */
O_Stream& dec (O_Stream& os);

/**
 * @brief wählt das hexadezimale Zahlensystem aus.
 * 
 * @param os 
 * @return O_Stream& 
 */
O_Stream& hex (O_Stream& os);


#endif

