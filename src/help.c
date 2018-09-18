#include <conio.h>
#ifdef __SPECTRUM__
#include <spectrum.h>
#endif
#include "help.h"

#define ENTRY_LENGTH 20

/** 
 * Clear help line
 */
void help_clear(void)
{
  gotoxy(1,22);
  textcolor(WHITE);
  textbackground(BLACK);
  cprintf("                                                               ");
  gotoxy(1,22);
}

/**
 * Show the input prompt for help 
 */
void help_prompt_input(const char* text)
{
  help_clear();
  textcolor(WHITE);
  textbackground(BLACK);
  cprintf(text);
}

/**
 * Show the resulting key gained from prompt input
 */
void help_prompt_key_display(const char* text, char extend)
{
  gotoxy(32,22);
  cprintf(" - is: ");
  if (extend==1)
    cprintf("EXTEND ");
  cprintf(text);
}

/**
 * Show available PLATO keys to get help.
 */
void help_keys(void)
{
  gotoxy(1,19);
  cprintf("                                                               ");
  cprintf("                                                               ");
  gotoxy(1,19);
  cprintf(" Available keys: ANS  BACK  COPY  DATA  EDIT  FONT  DIVIDE     ");
  cprintf(" HELP  LAB  MICRO  NEXT  SQUARE  STOP  SUB  SUPER  TERM  ASSIGN");
  
}

/**
 * Return if entered string matches help entry
 */
void help_matches(char* entry, const char* match)
{
  unsigned char i=0;
  // Translate to uppercase.
  for (i=0;i<ENTRY_LENGTH;i++)
    {
      entry[i]=toupper(entry[i]);
    }
  return strcmp(entry,match);
}

/**
 * Help entrypoint 
 */
void help_run(void)
{
  char help_done=1;
  char entry[ENTRY_LENGTH];
#ifdef __SPECTRUM__
  zx_border(YELLOW);
#endif
  help_keys();
    do {
      help_prompt_input(" Help for which key? ");
      cgets(entry);
      if (help_matches(entry,"KEYS")==0)
	{
	  help_keys();
	  help_done=0; // Repeat help after keys.
	}
      else if (help_matches(entry,"ANS")==0)
	{
	  help_prompt_key_display("a",1);
	  help_done=1;
	}
      else if (help_matches(entry,"BACK")==0)
	{
	  help_prompt_key_display("b",1);
	  help_done=1;
	}
      else if (help_matches(entry,"COPY")==0)
	{
	  help_prompt_key_display("c",1);
	  help_done=1;
	} 
      else if (help_matches(entry,"DATA")==0)
	{
	  help_prompt_key_display("d",1);
	  help_done=1;
	}
      else if (help_matches(entry,"EDIT")==0)
	{
	  help_prompt_key_display("e",1);
	  help_done=1;
	}
      else if (help_matches(entry,"FONT")==0)
	{
	  help_prompt_key_display("f",1);
	  help_done=1;
	}
      else if (help_matches(entry,"DIVIDE")==0)
	{
	  help_prompt_key_display("g",1);
	  help_done=1;
	}
      else if (help_matches(entry,"HELP")==0)
	{
	  help_prompt_key_display("h",1);
	  help_done=1;
	}
      else if (help_matches(entry,"LAB")==0)
	{
	  help_prompt_key_display("l",1);
	  help_done=1;
	}
      else if (help_matches(entry,"MICRO")==0)
	{
	  help_prompt_key_display("m",1);
	  help_done=1;
	}
      else if (help_matches(entry,"SUPER")==0)
	{
	  help_prompt_key_display("p",1);
	  help_done=1;
	}
      else if (help_matches(entry,"NEXT")==0)
	{
	  help_prompt_key_display("ENTER",0);
	  help_done=1;
	}
      else if (help_matches(entry,"SQUARE")==0)
	{
	  help_prompt_key_display("q",1);
	  help_done=1;
	}
      else if (help_matches(entry,"ERASE")==0)
	{
	  help_prompt_key_display("DELETE",0);
	  help_done=1;
	}
      else if (help_matches(entry,"STOP")==0)
	{
	  help_prompt_key_display("s",1);
	  help_done=1;
	}
      else if (help_matches(entry,"TERM")==0)
	{
	  help_prompt_key_display("t",1);
	  help_done=1;
	}
      else if (help_matches(entry,"SUB")==0)
	{
	  help_prompt_key_display("y",1);
	  help_done=1;
	}
      else if (help_matches(entry,"ASSIGN")==0)
	{
	  help_prompt_key_display("1",1);
	  help_done=1;
	}
      else
	{
	  help_prompt_key_display(" not a PLATO key.",0);
	}
    }   while (help_done==0);
#ifdef __SPECTRUM__
    zx_border(BLACK);
#endif
}
