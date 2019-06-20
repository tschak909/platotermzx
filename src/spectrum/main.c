#ifdef __SPECTRUM__

#include <stdbool.h>
#include <input.h>
#include <stdint.h>
#include <stdio.h>
#include "protocol.h"
#include "screen.h"
#include "terminal.h"
#include "connect.h"
#include "spectrum/splash.h"
#include "help.h"
#include <conio.h>

#include "io.h"
#include "keyboard.h"

#include "sound.h"	//Added sound for Keyboard and ready beep

unsigned char already_started=0;

void main(void)
{
  char c; 
  zx_border(INK_BLACK);  //Tidy up the borders on start up
  screen_init();
  terminal_init();
  ShowPLATO(splash,sizeof(splash));
  terminal_ready();
  terminal_initial_position();
  
  help_clear();
  cprintf("Enable graphics fill?(y/n)");
  while(1)
  {
    c = getch();
    if (c == 'y') {
      enable_fill = 1;
      break;
    }
    if (c == 'n')  {
      enable_fill = 0;
      break;
    }
  }

#ifdef __SPECTRANET__
  connect();
#endif
#ifdef __UNO__
  connect();
#endif
  io_init();
  bit_play("2A--");  //Ready beep
  
  for (;;)
    {
      keyboard_main();
      io_main();
    }
}

#endif /* __SPECTRUM__ */
