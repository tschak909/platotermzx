
#include <graphics.h>
#include <sound.h>
#include <stdlib.h>
#include "screen.h"
#include "protocol.h"

unsigned char CharWide=8;
unsigned char CharHigh=16;
padPt TTYLoc;

extern padBool FastText; /* protocol.c */
extern unsigned short scalex[];
extern unsigned short scaley[];

extern unsigned char font[];
extern unsigned char fontm23[];
extern unsigned short fontptr[];
extern unsigned char FONT_SIZE_X;
extern unsigned char FONT_SIZE_Y;

char enable_fill;

void bx(int x1, int y1, int x2, int y2)
{
  int y=0;
  for (y=y1; y<y2; y++)
    {
      undraw(x1,y,x2,y);
    }
}

/**
 * screen_wait(void) - Sleep for approx 16.67ms
 */
void screen_wait(void)
{
}

/**
 * screen_block_draw(Coord1, Coord2) - Perform a block fill from Coord1 to Coord2
 */
void screen_block_draw(padPt* Coord1, padPt* Coord2)
{
  if (CurMode==ModeErase || CurMode==ModeInverse)
    bx(scalex[Coord1->x],scaley[Coord1->y],scalex[Coord2->x],scaley[Coord2->y]);
  else
    bx(scalex[Coord1->x],scaley[Coord1->y],scalex[Coord2->x],scaley[Coord2->y]);
}

/**
 * screen_dot_draw(Coord) - Plot a mode 0 pixel
 */
void screen_dot_draw(padPt* Coord)
{
  if (CurMode==ModeErase || CurMode==ModeInverse)
    unplot(scalex[Coord->x],scaley[Coord->y]);
  else
    plot(scalex[Coord->x],scaley[Coord->y]);
}

/**
 * screen_line_draw(Coord1, Coord2) - Draw a mode 1 line
 */
void screen_line_draw(padPt* Coord1, padPt* Coord2)
{
  unsigned char x1=scalex[Coord1->x];
  unsigned char x2=scalex[Coord2->x];
  unsigned char y1=scaley[Coord1->y];
  unsigned char y2=scaley[Coord2->y];

  unsigned short x,y;
  unsigned char* aaddr;
  
  if (CurMode==ModeErase || CurMode==ModeInverse)
    undraw(scalex[Coord1->x],scaley[Coord1->y],scalex[Coord2->x],scaley[Coord2->y]);
  else
    draw(scalex[Coord1->x],scaley[Coord1->y],scalex[Coord2->x],scaley[Coord2->y]);
}

/**
 * screen_tty_char - Called to plot chars when in tty mode
 */
void screen_tty_char(padByte theChar)
{
  if ((theChar >= 0x20) && (theChar < 0x7F)) {
    screen_char_draw(&TTYLoc, &theChar, 1);
    TTYLoc.x += CharWide;
  }
  else if ((theChar == 0x0b)) /* Vertical Tab */
    {
      TTYLoc.y += CharHigh;
    }
  else if ((theChar == 0x08) && (TTYLoc.x > 7))	/* backspace */
    {
      TTYLoc.x -= CharWide;
      /* screen_block_draw(&scalex[TTYLoc.x],&scaley[TTYLoc.y],&scalex[TTYLoc.x+CharWide],&scaley[TTYLoc.y+CharHigh]); */
    }
  else if (theChar == 0x0A)			/* line feed */
    TTYLoc.y -= CharHigh;
  else if (theChar == 0x0D)			/* carriage return */
    TTYLoc.x = 0;
  
  if (TTYLoc.x + CharWide > 511) {	/* wrap at right side */
    TTYLoc.x = 0;
    TTYLoc.y -= CharHigh;
  }
  
  if (TTYLoc.y < 0) {
    screen_clear();
    TTYLoc.y=495;
  }

}


/**
 * Flood fill
 */
void screen_paint(padPt* Coord)
{
  if (enable_fill) 
    fill(scalex[Coord->x],scaley[Coord->y]);
}

/**
 * screen_done()
 */
void screen_done(void)
{
}
