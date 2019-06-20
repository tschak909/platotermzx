#ifdef __SPECTRUM_RS232__
#include <stdio.h>
#include <string.h>
#include <rs232.h>
#include <spectrum.h>
#include "io.h"
#include "protocol.h"

static unsigned char inb;

char io_initialized=0;

void io_init(void)
{
  rs232_params(RS_BAUD_9600|RS_STOP_1|RS_BITS_8,RS_PAR_NONE);  //  Bauds tested 1200[/] 2400[/] 4800[/] 9600[/] 19200[X] 38400[X] 57600[] 115200[] 
  rs232_init();
  io_initialized=1;
}

void io_send_byte(unsigned char b)
{
  if (io_initialized==1)
    {
      rs232_put(b);	//*IRQ-OFF (SENDING DATA)
    }
}

void io_main(void)
{
  if (rs232_get(&inb) != RS_ERR_NO_DATA)  	// *IRQ-OFF (RECEIVING DATA)
    {	/* [RX - Display] */ 	
      ShowPLATO(&inb,1);
    }
  else
    {  /* [NO RX - KEY scan] */  
      for(int Kscan=0;Kscan<30;Kscan++)  //Extra keyboard scanning					
	{
	  keyboard_main();
	} 
    }
}
#endif /* __SPECTRUM_RS232__ */
