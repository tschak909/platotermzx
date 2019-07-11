#ifdef __PC8801__

#include <graphics.h>
#include <sound.h>
#include <stdlib.h>
#include "../include/screen.h"
#include "../include/protocol.h"

extern unsigned char CharWide;
extern unsigned char CharHigh;
extern padPt TTYLoc;
extern padBool FastText; /* protocol.c */
extern unsigned short scalex[];
extern unsigned short scaley[];
extern unsigned char font[];
extern unsigned char fontm23[];
extern unsigned short fontptr[];
extern unsigned char FONT_SIZE_X;
extern unsigned char FONT_SIZE_Y;
extern char enable_fill;


/**
 * screen_init() - Set up the screen
 */
void screen_init(void)
{
  clg();
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
  unsigned char* aaddr;
  
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

  x=scalex[(Coord->x)];
  y=scaley[(Coord->y)+15];
  
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
 * screen_foreground - Set foreground
 */
void screen_foreground(padRGB* theColor)
{
}

/**
 * screen_background - Set Background
 */
void screen_background(padRGB* theColor)
{
}

#endif /* __PC8801__ */
