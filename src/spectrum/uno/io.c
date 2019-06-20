#ifdef __UNO__
#include <stdio.h>
#include <string.h>
#include <spectrum.h>
#include "io.h"
#include "protocol.h"
#include "spectrum/uno/zifi.h"

static unsigned char inb;

char io_initialized=0;
extern unsigned char is_extend;  //bring in is_extend for borders

void io_init(void)
{
  /* no init for zifi */
  io_initialized=1;
}

void io_send_byte(unsigned char b)
{
  if (io_initialized==1) {
    sendByte(b);
  }
}

void io_main(void)
{
  while (isAvail()) {
    inb = getByte();
    ShowPLATO(&inb,1);
  }
}

#endif /* __UNO__ */
