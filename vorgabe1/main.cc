/* $Id$ */

#include <bootloader/bootloader_info.h>
#include <machine/efifb.h>
#include <object/fb.h>
#include <machine/efiscr.h>
#include <device/efistr.h>
#include <machine/keyctrl.h>

extern BootloaderInfo *bootloader_info;

EFI_Stream kout;

int main()
{

 
  // short num = 10;
  // short num1 = 100;
  // short num2 = 500;
  // short num3 = 5000;

  // kout << "  asdasdasd  " << -5 << " s dsdfsdfsdfsdfsdfsdfsdfsdf" << endl; 
  // kout << "JA!" << endl; 

  // for(short i = 0; i < 2; i++)
  // {
  //   kout << hex << "hex " << num << ' ' << num1 << ' ' << num2 << ' ' << num3 << " i: " << i << endl;// << " bla\nbla" << '\n';
  //   kout << bin << "bin " << num << ' ' << num1 << ' ' << num2 << ' ' << num3 << " i: " << i << endl;// << " bla\nbla" << '\n';
  //   kout << dec << "dec " << num << ' ' << num1 << ' ' << num2 << ' ' << num3 << " i: " << i << endl;// << " bla\nbla" << '\n';
  //   kout << oct << "oct " << num << ' ' << num1 << ' ' << num2 << ' ' << num3 << " i: " << i << " " << endl;// << " bla\nbla" << '\n';
  // }

  bool is_hit = false;

  Keyboard_Controller contr = Keyboard_Controller();

  
  Key pressed;

  contr.set_repeat_rate(0,30);
  while (!is_hit &&  pressed.ascii() != 's')
  {
    
    pressed = contr.key_hit();
    if(pressed.valid()){
      
      kout << pressed.ascii();

      kout.flush();
    
      //is_hit = true;
      


    }
  }
  contr.set_repeat_rate(1,4);


  contr.set_led(4, true);

  while (!is_hit)
  {
    
    pressed = contr.key_hit();
    if(pressed.valid()){
      
      kout << pressed.ascii();
      kout.flush();
      


    }
  }

 
  return 0;
}
