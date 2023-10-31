/* $Id$ */
#include <bootloader/bootloader_info.h>
#include <machine/efifb.h>
#include <object/fb.h>
#include <machine/efiscr.h>

/* TODO: Hier muesst ihr selbst Code vervollstaendigen */

extern BootloaderInfo *bootloader_info;
        
int main()
{

  EFI_Framebuffer EFI_fb;
  
  Framebuffer fb(bootloader_info->horizontal_resolution, bootloader_info->vertical_resolution);

  // for (unsigned int y = 0; y < 50; y++)
  // {
  //     for (unsigned int x = 0; x < 50; x++)
  //     {
  //         unsigned int value = 0xffffff;
  //         fb.set_pixel(x, y, Pixel(value));
  //     }
  // }
  

  // EFI_fb.blit_rect(fb, 0, 0, 50, 50, 0, 0);

  EFI_Screen EFI_sc;

  int length = 26*2;
  // EFI_sc.print("A\nB\nC\nD\nE\nF\nG\nH\nI\nJ\nK\nL\nM\nN\nO\nP\nQ\nR\nS\nT\nU\nV\nW\nX\nY\nZ", length, Pixel(0xffffff), Pixel(0x000000));
  
  //length=5;
  //EFI_sc.print("ABC", length, Pixel(0xffffff), Pixel(0x000000));
  
  length = 26;

  for (int i = 0; i < 150; i++)
  {
    EFI_sc.print("ABCDEFGHIJKLMNOPQRSTUVWXYZ", length, Pixel(0xffffff), Pixel(0x000000));
  }

  //length = 28*2 - 1;
  //EFI_sc.print("A\nB\nC\nD\nE\nF\nG\nH\nI\nJ\nK\nL\nM\nN\nO\nP\nQ\nR\nS\nT\nU\nV\nW\nX\nY\nZ\nA\nB", length, Pixel(0xffffff), Pixel(0x000000));
  //EFI_sc.show(0, 0, 'A', Pixel(0xffffff), Pixel(0x000000));
  // EFI_sc.show(100, 0, 'W', Pixel(0xffffff), Pixel(0x000000));
  // EFI_sc.show(300, 0, 'x', Pixel(0xffffff), Pixel(0x000000));
 
  return 0;
}
