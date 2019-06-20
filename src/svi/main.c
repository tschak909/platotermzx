#ifdef __SVI__

#include <stdbool.h>
#include <input.h>
#include <stdint.h>
#include <stdio.h>
#include "protocol.h"
#include "screen.h"
#include "terminal.h"
#include "connect.h"
#include "svi/splash.h"
#include "help.h"
#include <conio.h>

#include "io.h"
#include "keyboard.h"

unsigned char already_started=0;

void main(void)
{
  char c; 
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

  io_init();
  
  for (;;)
    {
      keyboard_main();
      io_main();
    }
}

#endif /* __SVI__ */
