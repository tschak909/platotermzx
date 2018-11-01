/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *
 * keyboard_base.c - Keyboard functions (base)
 */

#ifdef __SPECTRUM__
#include <conio.h>
#include <spectrum.h>
#endif
#include "key.h"
#include "keyboard.h"
#include "screen.h"
#include "protocol.h"
#include "io.h"
#include "plato_key.h"
#include "help.h"

#include "sound.h"	//Added sound

extern padBool TTY;
static unsigned char ch;
unsigned char is_extend=0;  //deleted static is used in IO.C now for Rasta bars

/**
 * A simple key press feedback.
 */
void keyboard_click(void)
{
  unsigned char i,j;
  for (i=0;i<=10;i++)
    {
      bit_click();
      for (j=0;j<4;j++)
	{
	}
    }
}

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
	/* bit_fx4(0);  //Keyboard click  - until I find some thing better */
#ifndef __SVI__
      keyboard_click(); // maybe something better? ;) -thom
#endif
      if (is_extend==0 && ch==0x0e) // EXTEND pressed.
		{
#ifdef __SPECTRUM__
			zx_border(INK_GREEN);
#endif
			is_extend=1;
		}
      else if (TTY)
		{
			keyboard_out_tty(ch);	// *IRQ-OFF  (SENDING DATA)
		}
      else if (is_extend==1 && ch==0x30)
		{
			help_run();
		}
      else if (is_extend==1)
		{
#ifdef __SPECTRUM__
			zx_border(INK_GREEN);
#endif
			keyboard_out(extend_key_to_pkey[ch]);
			is_extend=0;
#ifdef __SPECTRUM__
			zx_border(INK_BLACK);
#endif
		}
      else
		{
#ifdef __SPECTRUM__
			zx_border(INK_BLACK);
#endif
			keyboard_out(key_to_pkey[ch]);
		}
    }
 //  ELSE here for No Keypress events
}

/**
 * keyboard_clear() - Clear the keyboard buffer
 */
void keyboard_clear(void)
{
}
