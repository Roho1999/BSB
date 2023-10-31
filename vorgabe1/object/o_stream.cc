/*****************************************************************************/
/* Betriebssysteme                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                            O _ S T R E A M                                */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Die Klasse O_Stream enthaelt die Definition des << Operators fuer die     */
/* wichtigsten der vordefinierten Datentypen und realisiert somit die        */
/* bekannte Ausgabefunktion der C++ IO_Stream Bibliothek. Zur Zeit wird die  */
/* Darstellung von Zeichen, Zeichenketten und ganzen Zahlen unterstuetzt.    */
/* Ein weiterer << Operator erlaubt die Verwendung von Manipulatoren.        */
/*                                                                           */
/* Neben der Klasse O_Stream sind hier auch die Manipulatoren hex, dec, oct  */
/* und bin fuer die Wahl der Basis bei der Zahlendarstellung, sowie endl     */
/* fuer den Zeilenumbruch definiert.                                         */
/*****************************************************************************/

#include "object/o_stream.h"

/* TODO: Hier muesst ihr selbst Code vervollstaendigen */ 
/**
     * @brief Anfügen des Zeichens c an die gesammelten Zeichen.
     * 
     */
    O_Stream& O_Stream::operator<< (unsigned char c){

    }
    O_Stream& O_Stream::operator<< (char c){

    }

    /**
     * @brief Anfügen der Zahl number in dem gewählten Zahlensystem.
     * 
     */
    O_Stream& O_Stream::operator<< (unsigned short number){

    }
    O_Stream& O_Stream::operator<< (short number){

    }
    O_Stream& O_Stream::operator<< (unsigned int number){

    }
    O_Stream& O_Stream::operator<< (int number){

    }
    O_Stream& O_Stream::operator<< (unsigned long number){

    }
    O_Stream& O_Stream::operator<< (long number){

    }

    /**
     * @brief Anfügen des Zeigerwertes pointer im Hexadezimalsystem.
     * 
     */
    O_Stream& O_Stream::operator<< (void* pointer){

    }

    /**
     * @brief Anfügen der nullterminierten Zeichenkette text (ohne die Nullterminierung).
     * 
     */
    
    O_Stream& O_Stream::operator<< (char* text){

    }


    

    /**
     * @brief  Aufruf der Manipulatorfunktion fkt
     * 
     * @param fkt 
     * @return O_Stream& 
     */
    O_Stream& O_Stream::operator<< (O_Stream& (*fkt) (O_Stream&)){

    }

    //Bei den Manipulatoren bin ich mir nicht komplett sicher wie man die genau deklariert

    /**
     * @brief fügt einen Zeilenumbruch ein.
     * 
     * @param os 
     * @return O_Stream& 
     */
    O_Stream& endl (O_Stream& os){

    }

    /**
     * @brief wählt das binäre Zahlensystem aus.
     * 
     * @param os 
     * @return O_Stream& 
     */
    O_Stream& bin (O_Stream& os){

    }

    /**
     * @brief wählt das oktale Zahlensystem aus.
     * 
     * @param os 
     * @return O_Stream& 
     */
    O_Stream& oct (O_Stream& os){

    }

    /**
     * @brief wählt das dezimale Zahlensystem aus.
     * 
     * @param os 
     * @return O_Stream& 
     */
    O_Stream& dec (O_Stream& os){

    }

    /**
     * @brief wählt das hexadezimale Zahlensystem aus.
     * 
     * @param os 
     * @return O_Stream& 
     */
    O_Stream& hex (O_Stream& os){

    }