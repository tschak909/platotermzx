/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * keyboard_base.c - Keyboard functions (base)
 */

#ifdef __MSX__

#include <conio.h>
#include "../include/key.h"
#include "../include/keyboard.h"
#include "../include/screen.h"
#include "../include/protocol.h"
#include "../include/io.h"
#include "../include/plato_key.h"
#include "../include/help.h"

#include "sound.h"	//Added sound

extern padBool TTY;
static unsigned char ch;
unsigned char is_extend=0;  //deleted static is used in IO.C now for Rasta bars

/**
 * keyboard_out - If platoKey < 0x7f, pass off to protocol
 * directly. Otherwise, platoKey is an access key, and the
 * ACCESS key must be sent, followed by the particular
 * access key from PTAT_ACCESS.
 */
void keyboard_out(unsigned char platoKey)
{
  if (platoKey==0xff)
    return;
  
  if (platoKey>0x7F)
    {
      Key(ACCESS);
      Key(ACCESS_KEYS[platoKey-0x80]);
      return;
    }
  Key(platoKey);
  return;
}

/**
 * keyboard_out_tty - keyboard output to serial I/O in TTY mode
 */
void keyboard_out_tty(char ch)
{
  io_send_byte(ch);  	// *IRQ-OFF  (SENDING DATA)
}

/**
 * keyboard_main - Handle the keyboard presses
 */
void keyboard_main(void)
{
  ch=getk();	
  if (ch!=0x00)
    {
      keyboard_out(key_to_pkey[ch]);
    }
}

#endif /* __MSX__ */
