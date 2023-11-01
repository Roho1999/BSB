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

  EFI_Stream kout;
  short num = 10;
  short num1 = 100;
  short num2 = 500;
  short num3 = 5000;

  for(short i = 0; i < 2; i++)
  {
    kout << hex << "hex " << num << ' ' << num1 << ' ' << num2 << ' ' << num3 << " i: " << i << "     "<< endl;// << " bla\nbla" << '\n';
    kout << bin << "bin " << num << ' ' << num1 << ' ' << num2 << ' ' << num3 << " i: " << i << "     "<< endl;// << " bla\nbla" << '\n';
    kout << dec << "dec " << num << ' ' << num1 << ' ' << num2 << ' ' << num3 << " i: " << i << "     "<< endl;// << " bla\nbla" << '\n';
    kout << oct << "oct " << num << ' ' << num1 << ' ' << num2 << ' ' << num3 << " i: " << i << "     " << endl;// << " bla\nbla" << '\n';
  }


  
 
  return 0;
}
