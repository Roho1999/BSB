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




int count_digit(long number)
{
   int count = 0;
   while(number != 0) {
      number = number / 10;
      count++;
   }
   return count;
}



void O_Stream::print_num(long number)
{
     unsigned int length = count_digit(number);

     int count = 0;
     char cs[length];
     unsigned int current;
     while(number != 0) {
          cs[length-count-1] = number % 10;
          number = number / 10;
          count++;
     }
     for (unsigned int i = 0; i < length; i++)
     {
          current = cs[i];
          char c = current + 48;
          this->put(c);
     }
}



int count_digit_hex(long number)
{
   int count = 0;
   while(number != 0) {
      number = number / 16;
      count++;
   }
   return count;
}


long conv_to_bin(long decimal)
{
     int binary = 0, remainder, product = 1;
     while (decimal != 0) {
          remainder = decimal % 2;
          binary = binary + (remainder * product);
          decimal = decimal / 2;
          product *= 10;
     }
     return binary;
}

long conv_to_oct(long decimal)
{
     int oct = 0, remainder, product = 1;
     while (decimal != 0) {
          remainder = decimal % 8;
          oct = oct + (remainder * product);
          decimal = decimal / 8;
          product *= 10;
     }
     return oct;
}

void O_Stream::handle_hex(long dec)
{
     int length = count_digit_hex(dec);
     char s[length];
     int rem;
     int counter = 0;
     while (dec > 0)   // Do this whilst the quotient is greater than 0.
     {
          rem = dec % 16; // Get the remainder.
          if (rem > 9)
          {
               // Map the character given that the remainder is greater than 9.
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
     for (unsigned int i = 0; i < length; i++)
     {
          current = s[i];
          this->put(current);
     }
}

long conv_to_mode(O_Stream::Mode mode, long number)
{
     switch (mode)
     {
     case O_Stream::bin:
          number = conv_to_bin(number);
          break;
     case O_Stream::oct:
          number = conv_to_oct(number);
          break; 
     default:
          break;
     }
     return number;
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
     if (this->mode == hex)
     {
          handle_hex(number);
     }
     else
     {
          number = conv_to_mode(this->mode, number);
          print_num(number);
     }
     
     return *this;
}

O_Stream& O_Stream::operator<< (short number){
     if (this->mode == hex)
     {
          handle_hex(number);
     }
     else
     {
          number = conv_to_mode(this->mode, number);
          print_num(number);
     }
     
     return *this;
}

O_Stream& O_Stream::operator<< (unsigned int number){
     if (this->mode == hex)
     {
          handle_hex(number);
     }
     else
     {
          number = conv_to_mode(this->mode, number);
          print_num(number);
     }
     
     return *this;
}

O_Stream& O_Stream::operator<< (int number){
     if (this->mode == hex)
     {
          handle_hex(number);
     }
     else
     {
          number = conv_to_mode(this->mode, number);
          print_num(number);
     }
     
     return *this;
}

O_Stream& O_Stream::operator<< (unsigned long number){
     if (this->mode == hex)
     {
          handle_hex(number);
     }
     else
     {
          number = conv_to_mode(this->mode, number);
          print_num(number);
     }
     
     return *this;
}

O_Stream& O_Stream::operator<< (long number){
     if (this->mode == hex)
     {
          handle_hex(number);
     }
     else
     {
          number = conv_to_mode(this->mode, number);
          print_num(number);
     }
     
     return *this;
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