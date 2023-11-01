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



void O_Stream::handle_dec(long dec)
{
     int length = 0;
     long temp = dec;
     while(temp != 0) {
          temp = temp / 10;
          length++;
     }
     if (length == 0)
     {
          this->put('0');
     }

     char s[length];
     int rem;
     int counter = 0;
     while (dec > 0)
     {
          rem = dec % 10;
          s[length-counter-1] = (char) (rem + 48);
          dec = dec/10;
          counter++;
     }
     
     char current;
     for (int i = 0; i < length; i++)
     {
          current = s[i];
          this->put(current);
     }
}


void O_Stream::handle_oct(long dec)
{
     int length = 0;
     long temp = dec;
     while(temp != 0) {
          temp = temp / 8;
          length++;
     }
     if (length == 0)
     {
          this->put('0');
     }

     char s[length];
     int rem;
     int counter = 0;
     while (dec > 0)
     {
          rem = dec % 8;
          s[length-counter-1] = (char) (rem + 48);
          dec = dec/8;
          counter++;
     }
     
     char current;
     for (int i = 0; i < length; i++)
     {
          current = s[i];
          this->put(current);
     }
}


void O_Stream::handle_bin(long dec)
{
     int length = 0;
     long temp = dec;
     while(temp != 0) {
          temp = temp / 2;
          length++;
     }
     if (length == 0)
     {
          this->put('0');
     }

     char s[length];
     int rem;
     int counter = 0;
     while (dec > 0)
     {
          rem = dec % 2;
          s[length-counter-1] = (char) (rem + 48);
          dec = dec/2;
          counter++;
     }

     char current;
     for (int i = 0; i < length; i++)
     {
          current = s[i];
          this->put(current);
     }
}


void O_Stream::handle_hex(long dec)
{

     int length = 0;
     long temp = dec;
     while(temp != 0) {
          temp = temp / 16;
          length++;
     }
     if (length == 0)
     {
          this->put('0');
     }

     char s[length];
     int rem;
     int counter = 0;
     while (dec > 0)
     {
          rem = dec % 16;
          if (rem > 9)
          {
               switch (rem)
               {
                    case 10: s[length-counter-1] = 'A'; break;
                    case 11: s[length-counter-1] = 'B'; break;
                    case 12: s[length-counter-1] = 'C'; break;
                    case 13: s[length-counter-1] = 'D'; break;
                    case 14: s[length-counter-1] = 'E'; break;
                    case 15: s[length-counter-1] = 'F'; break;
               }
          }
          else
          {
               s[length-counter-1] = (char) (rem + 48);
          }
          dec = dec/16;
          counter++;
     }
     char current;
     for (int i = 0; i < length; i++)
     {
          current = s[i];
          this->put(current);
     }
}


void O_Stream::conv_to_mode(O_Stream::Mode mode, long number)
{
     switch (mode)
     {
     case O_Stream::bin:
          handle_bin(number);
          break;
     case O_Stream::oct:
          handle_oct(number);
          break; 
     case O_Stream::hex:
          handle_hex(number);
          break; 
     case O_Stream::dec:
          handle_dec(number);
          break;
     default:
          break; 
     }
}


/**
* @brief Anfügen des Zeichens c an die gesammelten Zeichen.
* 
*/
O_Stream& O_Stream::operator<< (unsigned char c){
     this->put(c);
     return *this;
}

O_Stream& O_Stream::operator<< (char c){
     this->put(c);
     return *this;
}

/**
* @brief Anfügen der Zahl number in dem gewählten Zahlensystem.
* 
*/
O_Stream& O_Stream::operator<< (unsigned short number){
     conv_to_mode(this->mode, number);
     return *this;
}

O_Stream& O_Stream::operator<< (short number){
     if (number < 0)
     {
          number *= -1;
          this->put('-');
     }
     conv_to_mode(this->mode, number);
     return *this;
}

O_Stream& O_Stream::operator<< (unsigned int number){
     conv_to_mode(this->mode, number);
     return *this;
}

O_Stream& O_Stream::operator<< (int number){
     if (number < 0)
     {
          number *= -1;
          this->put('-');
     }
     conv_to_mode(this->mode, number);
     return *this;
}

O_Stream& O_Stream::operator<< (unsigned long number){
     conv_to_mode(this->mode, number);
     return *this;
}

O_Stream& O_Stream::operator<< (long number){
     if (number < 0)
     {
          number *= -1;
          this->put('-');
     }
     conv_to_mode(this->mode, number);
     return *this;
}

/**
* @brief Anfügen des Zeigerwertes pointer im Hexadezimalsystem.
* 
*/
O_Stream& O_Stream::operator<< (void* pointer){
     return *this;
}

/**
* @brief Anfügen der nullterminierten Zeichenkette text (ohne die Nullterminierung).
* 
*/

O_Stream& O_Stream::operator<< (char* text){

     for(unsigned int i = 0; text[i] != '\0'; i++)
     {
          this->put(text[i]);
     }
     return *this;
}


/**
* @brief  Aufruf der Manipulatorfunktion fkt
* 
* @param fkt 
* @return O_Stream& 
*/
O_Stream& O_Stream::operator<< (O_Stream& (*fkt) (O_Stream&)){
     return (*fkt)(*this);
}

//Bei den Manipulatoren bin ich mir nicht komplett sicher wie man die genau deklariert

/**
* @brief fügt einen Zeilenumbruch ein.
* 
* @param os 
* @return O_Stream& 
*/
O_Stream& endl (O_Stream& os){
     os.put('\n');
     os.flush();
     return os;
}

/**
* @brief wählt das binäre Zahlensystem aus.
* 
* @param os 
* @return O_Stream& 
*/
O_Stream& bin (O_Stream& os){
     os.mode = os.bin;
     return os;
}

/**
* @brief wählt das oktale Zahlensystem aus.
* 
* @param os 
* @return O_Stream& 
*/
O_Stream& oct (O_Stream& os){
     os.mode = os.oct;
     return os;
}

/**
* @brief wählt das dezimale Zahlensystem aus.
* 
* @param os 
* @return O_Stream& 
*/
O_Stream& dec (O_Stream& os){
     os.mode = os.dec;
     return os;
}

/**
* @brief wählt das hexadezimale Zahlensystem aus.
* 
* @param os 
* @return O_Stream& 
*/
O_Stream& hex (O_Stream& os){
     os.mode = os.hex;
     return os;
}