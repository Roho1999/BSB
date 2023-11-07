/* $Id$ */
#include <bootloader/bootloader_info.h>
#include <machine/efifb.h>
#include <object/fb.h>
#include <machine/efiscr.h>
#include <device/efistr.h>
#include <machine/keyctrl.h>

/* TODO: Hier muesst ihr selbst Code vervollstaendigen */

extern BootloaderInfo *bootloader_info;

EFI_Stream kout;

int main()
{

 
  // short num = 10;
  // short num1 = 100;
  // short num2 = 500;
  // short num3 = 5000;

  // kout << "  asdasdasd  " << -5 << " s dsdfsdfsdfsdfsdfsdfsdfsdf" <<endl; 

  // for(short i = 0; i < 10; i++)
  // {
  //   kout << hex << "hex " << num << ' ' << num1 << ' ' << num2 << ' ' << num3 << " i: " << i << "     "<< endl;// << " bla\nbla" << '\n';
  //   kout << bin << "bin " << num << ' ' << num1 << ' ' << num2 << ' ' << num3 << " i: " << i << "     "<< endl;// << " bla\nbla" << '\n';
  //   kout << dec << "dec " << num << ' ' << num1 << ' ' << num2 << ' ' << num3 << " i: " << i << "     "<< endl;// << " bla\nbla" << '\n';
  //   kout << oct << "oct " << num << ' ' << num1 << ' ' << num2 << ' ' << num3 << " i: " << i << "     " << endl;// << " bla\nbla" << '\n';
  // }
    
  Keyboard_Controller kc;
  while(true){
    Key valid = kc.key_hit();
    valid.ascii('z');
    //kout << 'w' << endl;
    
    ///break;
    if(valid.valid()){
      kout << valid.ascii() << endl;
    }
    
  }
  
 
  return 0;
}
