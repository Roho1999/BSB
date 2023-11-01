#ifndef __efi_screen_include__
#define __efi_screen_include__

/*****************************************************************************/
/* Operating System Construction                                             */
/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                             E F I _ S C R E E N                           */
/*                                                                           */
/*---------------------------------------------------------------------------*/
/* By using this class you can access the screen of the PC. Access is        */
/* performed directly in the hardware level by writing to the graphic card's */
/* framebuffer, which is mapped into the kernel's address space.             */
/* In order to improve the performance, the screen content is stored in a    */
/* fully-cached local buffer. Whenever changes in the local buffer must      */
/* become visible on the screen, the hardware frame buffer is updated.       */
/* The class supports printing ASCII-encoded text and a visible text cursor. */
/*****************************************************************************/

#include "object/psffont.h"
#include "object/fb.h"
#include "machine/efifb.h"
/* TODO: Hier muesst ihr selbst Code vervollstaendigen */ 

class EFI_Screen {
public: 
   EFI_Screen ();
   void clear();
   int getrows () const;
   int getcolumns() const;
   void show (int x, int y, unsigned char c, Pixel fg_color, Pixel bg_color);
   void setpos (int x, int y);
   void getpos (int &x, int &y) const;
   void print (const char* text, Pixel fg_color, Pixel bg_color);
   
private:
   EFI_Screen(const EFI_Screen &copy) = delete; // Prohibit copying
   void scroll_up();
   EFI_Framebuffer efifb;
   Framebuffer fb;
   unsigned int rows; 
   unsigned int posx;
   unsigned int posy;
   unsigned int columns;
   
   
/* TODO: Hier muesst ihr selbst Code vervollstaendigen */ 
 };

#endif
