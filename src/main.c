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
		//zx_border(INK_CYAN);  //Debug timing
		for(int Kscan=0;Kscan<40;Kscan++)			//  <TIME>*IRQ STATE [EXECUTION PATH]
		  {											// 	[NO KEY]			[KEY Local]		[KEY TTY]
			keyboard_main();						// 	[<1>*IRQ-ON] 	[<4>*IRQ-ON] 	[<80>*IRQ-OFF]
		  }
		io_main();									//	[NO RX TTY - KEY scan]			[RX TTY	- Draw Screen]
													//	[<10>*IRQ-OFF - <40>*IRQ-ON]	[<80>*IRQ-OFF - <20>*IRQ-ON]
    }
}
