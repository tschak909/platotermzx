/**
 * PLATOTerm64 - A PLATO Terminal for the Commodore 64
 * Based on Steve Peltz's PAD
 * 
 * Author: Thomas Cherryhomes <thom.cherryhomes at gmail dot com>
 *         Owen Reynolds 
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

extern padBool TTY;
static unsigned char ch;
static unsigned char is_extend=0;

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
  io_send_byte(ch);  // *IRQ - OFF
}

/**
 * keyboard_main - Handle the keyboard presses
 */
void keyboard_main(void)
{
  ch=getk();	//  <TIME>*IRQ STATE [EXECUTION PATH]
  if (ch!=0x00) // [KEY Local]		[KEY TTY] 				//Path: ( [KEY Local <1-4>*IRQ-ON] || [KEY TTY <80>*IRQ-OFF] ) || [NO Key hit <1>*IRQ-ON]
    {
      if (is_extend==0 && ch==0x0e) // EXTEND pressed.		// <2>*IRQ-ON 
		{
#ifdef __SPECTRUM__
			zx_border(INK_GREEN);
#endif
			is_extend=1;
		}
      else if (TTY)  										// <80>*IRQ-OFF  (SENDING DATA)
		{
			keyboard_out_tty(ch);
		}
      else if (is_extend==1 && ch==0x30)					// <1>*IRQ-ON
		{
			help_run();
		}
      else if (is_extend==1)								// <4>*IRQ-ON
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
      else													// <3>*IRQ-ON 
		{
#ifdef __SPECTRUM__
			zx_border(INK_BLACK);
#endif
			keyboard_out(key_to_pkey[ch]);
		}
    }
  else  		//  [NO KEY]								// <1>*IRQ-ON 
    {
	printf("");		//  delay  --  May be remove this and add a few more to the keyboard scan loops
    }
}

/**
 * keyboard_clear() - Clear the keyboard buffer
 */
void keyboard_clear(void)
{
}
