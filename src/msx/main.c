#ifdef __MSX__

#include <stdbool.h>
#include <input.h>
#include <stdint.h>
#include <stdio.h>
#include "../include/protocol.h"
#include "../include/screen.h"
#include "../include/terminal.h"
#include "../include/connect.h"
#include "splash.h"
#include "../include/help.h"
#include <conio.h>

#include "../include/io.h"
#include "../include/keyboard.h"

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
  
  io_init();
  
  for (;;)
    {
      keyboard_main();
      io_main();
    }
}

#endif /* __MSX__ */
