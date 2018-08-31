
#include <graphics.h>
#include <spectrum.h>
#include "screen.h"
#include "protocol.h"

unsigned char CharWide=8;
unsigned char CharHigh=16;
padPt TTYLoc;
long foregroundColor;
long backgroundColor;

extern padBool FastText; /* protocol.c */
extern unsigned short scalex[];
extern unsigned short scaley[];

extern unsigned char font[];
extern unsigned char fontm23[];
extern unsigned short fontptr[];
extern unsigned char FONT_SIZE_X;
extern unsigned char FONT_SIZE_Y;


void bx(int x1, int y1, int x2, int y2)
{
  int y=0;
  for (y=y1; y<y2; y++)
    {
      undraw(x1,y,x2,y);
    }
}

/**
 * screen_init() - Set up the screen
 */
void screen_init(void)
{
  clg();
}

/**
 * screen_wait(void) - Sleep for approx 16.67ms
 */
void screen_wait(void)
{
}

/**
 * screen_beep(void) - Beep the terminal
 */
void screen_beep(void)
{
}

/**
 * screen_clear - Clear the screen
 */
void screen_clear(void)
{
  clg();
}

/**
 * screen_block_draw(Coord1, Coord2) - Perform a block fill from Coord1 to Coord2
 */
void screen_block_draw(padPt* Coord1, padPt* Coord2)
{
  /* short y; */

  /* if (CurMode==ModeErase || CurMode==ModeInverse) */
  /*   { */
  /*     undrawb(scalex[Coord1->x],scaley[Coord1->y],scalex[Coord2->x],scaley[Coord2->y]); */
  /*   } */
  /* else */
  /*   { */
  /*    drawb(scalex[Coord1->x],scaley[Coord1->y],scalex[Coord2->x],scaley[Coord2->y]); */
  /*   } */
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
  unsigned short x1=scalex[Coord1->x];
  unsigned short x2=scalex[Coord2->x];
  unsigned short y1=scaley[Coord1->y];
  unsigned short y2=scaley[Coord2->y];

  if (CurMode==ModeErase || CurMode==ModeInverse)
    undraw(scalex[Coord1->x],scaley[Coord1->y],scalex[Coord2->x],scaley[Coord2->y]);
  else
    draw(scalex[Coord1->x],scaley[Coord1->y],scalex[Coord2->x],scaley[Coord2->y]);
}

/**
 * screen_char_draw(Coord, ch, count) - Output buffer from ch* of length count as PLATO characters
 */
void screen_char_draw(padPt* Coord, unsigned char* ch, unsigned char count)
{
  short offset; /* due to negative offsets */
  unsigned short x;      /* Current X and Y coordinates */
  unsigned short y;
  unsigned short* px;   /* Pointers to X and Y coordinates used for actual plotting */
  unsigned short* py;
  unsigned char i; /* current character counter */
  unsigned char a; /* current character byte */
  unsigned char j,k; /* loop counters */
  char b; /* current character row bit signed */
  unsigned char width=FONT_SIZE_X;
  unsigned char height=FONT_SIZE_Y;
  unsigned short deltaX=1;
  unsigned short deltaY=1;
  unsigned char mainColor=1;
  unsigned char altColor=0;
  unsigned char *p;
  unsigned char* curfont;
  
  switch(CurMem)
    {
    case M0:
      curfont=font;
      offset=-32;
      break;
    case M1:
      curfont=font;
      offset=64;
      break;
    case M2:
      curfont=fontm23;
      offset=-32;
      break;
    case M3:
      curfont=fontm23;
      offset=32;      
      break;
    }

  if (CurMode==ModeRewrite)
    {
      altColor=0;
    }
  else if (CurMode==ModeInverse)
    {
      altColor=1;
    }
  
  if (CurMode==ModeErase || CurMode==ModeInverse)
    mainColor=0;
  else
    mainColor=1;

  x=scalex[(Coord->x&0x1FF)];
  y=scaley[(Coord->y)+14&0x1FF];
  
  if (FastText==padF)
    {
      goto chardraw_with_fries;
    }

  /* the diet chardraw routine - fast text output. */
  
  for (i=0;i<count;++i)
    {
      a=*ch;
      ++ch;
      a+=offset;
      p=&curfont[fontptr[a]];
      
      for (j=0;j<FONT_SIZE_Y;++j)
  	{
  	  b=*p;
	  
  	  for (k=0;k<FONT_SIZE_X;++k)
  	    {
  	      if (b<0) /* check sign bit. */
		{
		  if (mainColor==0)
		    unplot(x,y);
		  else
		    plot(x,y);
		}

	      ++x;
  	      b<<=1;
  	    }

	  ++y;
	  x-=width;
	  ++p;
  	}

      x+=width;
      y-=height;
    }

  return;

 chardraw_with_fries:
  if (Rotate)
    {
      deltaX=-abs(deltaX);
      width=-abs(width);
      px=&y;
      py=&x;
    }
    else
    {
      px=&x;
      py=&y;
    }
  
  if (ModeBold)
    {
      deltaX = deltaY = 2;
      width<<=1;
      height<<=1;
    }
  
  for (i=0;i<count;++i)
    {
      a=*ch;
      ++ch;
      a+=offset;
      p=&curfont[fontptr[a]];
      for (j=0;j<FONT_SIZE_Y;++j)
  	{
  	  b=*p;

	  if (Rotate)
	    {
	      px=&y;
	      py=&x;
	    }
	  else
	    {
	      px=&x;
	      py=&y;
	    }

  	  for (k=0;k<FONT_SIZE_X;++k)
  	    {
  	      if (b<0) /* check sign bit. */
		{
		  if (ModeBold)
		    {
		      if (mainColor==0)
			{
			  unplot(*px+1,*py);
			  unplot(*px,*py+1);
			  unplot(*px+1,*py+1);
			}
		      else
			{
			  plot(*px+1,*py);
			  plot(*px,*py+1);
			  plot(*px+1,*py+1);
			}
		    }
		  if (mainColor==0)
		    unplot(*px,*py);
		  else
		    plot(*px,*py);
		}
	      else
		{
		  if (CurMode==ModeInverse || CurMode==ModeRewrite)
		    {
		      if (ModeBold)
			{
			  if (altColor==0)
			    {
			      unplot(*px+1,*py);
			      unplot(*px,*py+1);
			      unplot(*px+1,*py+1);
			    }
			  else
			    {
			      plot(*px+1,*py);
			      plot(*px,*py+1);
			      plot(*px+1,*py+1);
			    }			  
			}
		      if (altColor==0)
			unplot(*px,*py);
		      else
			plot(*px,*py);      
		    }
		}

	      x += deltaX;
  	      b<<=1;
  	    }

	  y+=deltaY;
	  x-=width;
	  ++p;
  	}

      Coord->x+=width;
      x+=width;
      y-=height;
    }

  return;
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
 * screen_foreground - Set foreground
 */
void screen_foreground(padRGB* theColor)
{
  unsigned char red=theColor->red;
  unsigned char green=theColor->green;
  unsigned char blue=theColor->blue;
  
  if (red==0 && green==0 && blue==0)
    {
      foregroundColor=INK_BLACK;
    }
  else if (red==0 && green==0 && blue==255)
    {
      foregroundColor=INK_BLUE;
    }
  else if (red==0 && green==255 && blue==0)
    {
      foregroundColor=INK_GREEN;
    }
  else if (red==255 && green==0 && blue==0)
    {
      foregroundColor=INK_RED;
    }
  else if (red==0 && green==255 && blue==255)
    {
      foregroundColor=INK_CYAN;
    }
  else if (red==255 && green==0 && blue==255)
    {
      foregroundColor=INK_MAGENTA;
    }
  else if (red==255 && green==255 && blue==0)
    {
      foregroundColor=INK_YELLOW;
    }
  else
    {
      foregroundColor=INK_WHITE;
    }
}

/**
 * screen_background - Set Background
 */
void screen_background(padRGB* theColor)
{
  unsigned char red=theColor->red;
  unsigned char green=theColor->green;
  unsigned char blue=theColor->blue;
  
  if (red==0 && green==0 && blue==0)
    {
      backgroundColor=PAPER_BLACK;
    }
  else if (red==0 && green==0 && blue==255)
    {
      backgroundColor=PAPER_BLUE;
    }
  else if (red==0 && green==255 && blue==0)
    {
      backgroundColor=PAPER_GREEN;
    }
  else if (red==255 && green==0 && blue==0)
    {
      backgroundColor=PAPER_RED;
    }
  else if (red==0 && green==255 && blue==255)
    {
      backgroundColor=PAPER_CYAN;
    }
  else if (red==255 && green==0 && blue==255)
    {
      backgroundColor=PAPER_MAGENTA;
    }
  else if (red==255 && green==255 && blue==0)
    {
      backgroundColor=PAPER_YELLOW;
    }
  else
    {
      backgroundColor=PAPER_BLACK;
    }
}

/**
 * screen_done()
 */
void screen_done(void)
{
}
