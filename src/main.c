#include <stdbool.h>
#include <input.h>
#include <stdint.h>
#include "protocol.h"
#include "screen.h"
#include "terminal.h"
#include "connect.h"
#include "splash.h"

unsigned char already_started=0;

void main(void)
{
  screen_init();
  terminal_init();
  ShowPLATO(splash,sizeof(splash));
  terminal_initial_position();
#ifdef __SPECTRANET__
  connect();
#endif
  io_init();
  for (;;)
    {
      keyboard_main();
      io_main();
    }
}
