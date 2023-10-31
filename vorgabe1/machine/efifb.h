#ifndef __efi_framebuffer_include__
#define __efi_framebuffer_include__

#include "object/fb.h"

class EFI_Framebuffer {
public:
   EFI_Framebuffer();
   void blit(const Framebuffer &fb, unsigned int x, unsigned int y);
   void clear();
   void blit_rect(const Framebuffer &fb, unsigned int xr, unsigned int yr, unsigned int wr, unsigned int hr, unsigned int x, unsigned int y);
   unsigned int get_width() const;
   unsigned int get_height() const;
private:
   EFI_Framebuffer(const EFI_Framebuffer &copy) = delete; // Prohibit copying
/* TODO: Hier muesst ihr selbst Code vervollstaendigen */
   unsigned int height;
   unsigned int width;
   unsigned int *buffer_ptr;
  
};

#endif // __efi_framebuffer_include__
