/* $Id$ */
#include <bootloader/bootloader_info.h>
#include <machine/efifb.h>
#include <object/fb.h>
#include <machine/efiscr.h>
#include <device/efistr.h>

/* TODO: Hier muesst ihr selbst Code vervollstaendigen */

extern BootloaderInfo *bootloader_info;
        
int main()
{

  //EFI_Framebuffer EFI_fb;
  
  //Framebuffer fb(bootloader_info->horizontal_resolution, bootloader_info->vertical_resolution);

  // for (unsigned int y = 0; y < 50; y++)
  // {
  //     for (unsigned int x = 0; x < 50; x++)
  //     {
  //         unsigned int value = 0xffffff;
  //         fb.set_pixel(x, y, Pixel(value));
  //     }
  // }
  

  // EFI_fb.blit_rect(fb, 0, 0, 50, 50, 0, 0);

  //EFI_Screen EFI_sc;

  // demonstriere Scrolling
  // unsigned int length = 26;
  // for (int i = 0; i < 150; i++)
  // {
  //   EFI_sc.print("ABCDEFGHIJKLMNOPQRSTUVWXYZ", length, Pixel(0xffffff), Pixel(0x000000));
  // }


  EFI_Stream kout;
  long num = 25;
  long num1 = 16;
  long num2 = 5;
  long num3 = 100;
  kout << hex << ' ' << num << ' ' << num1 << ' ' << num2 << ' ' << num3 << " blablablablablabalbalbalb";

  
 
  return 0;
}
