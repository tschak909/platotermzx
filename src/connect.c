
#include <conio.h>
#include <spectrum.h>
#include <string.h>
#include "connect.h"
#include "help.h"

extern char host_name[32];

/**
 * Connect to host prompt and input
 */
void connect(void)
{
  gotoxy(1,19);
  cprintf("Press EXTEND 0 for help with PLATO keys.");
  help_clear();
  help_prompt_input("Enter Hostname or <ENTER> for IRATA.ONLINE: ");
  cgets(host_name);
  if (strcmp(host_name,"")==0)
    {
      strcpy(host_name,"IRATA.ONLINE");
    }
  help_clear();
}
