/*****************************************************************************/
/* Betriebssysteme                                                           */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                            K E Y B O A R D                                */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* Tastaturtreiber.                                                          */
/*****************************************************************************/

#ifndef __Keyboard_include__
#define __Keyboard_include__

#include "machine/keyctrl.h"
#include "guard/gate.h"
#include "machine/key.h"
 
class Keyboard 
/* TODO: Hier muesst ihr selbst Code vervollstaendigen */         
{        
private:
      Keyboard(const Keyboard &other) = delete; // Verhindere Kopieren
      Keyboard& operator=(const Keyboard& other) = delete; // Verhindere Kopieren
/* TODO: Hier muesst ihr selbst Code vervollstaendigen */                 
         
public: 
 
      // PLUGIN: 'Anstoepseln' der Tastatur. Ab sofort werden Tasten erkannt.
      void plugin ();


      //This method responds to interrupts from the keyboard. 
      //Since several interrupts are triggered with each keystroke,
      // a usable ASCII code cannot always be expected.
      // Only if the method key_hit of the base class Keyboard_Controller delivers a valid Key,
      //a complete scancode could be determined. For "normal" keys, 
      //an ASCII code other than zero also exists. 
      void trigger ();
 };

#endif
