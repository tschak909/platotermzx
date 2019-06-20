#ifdef __SPECTRANET__
#include <conio.h>
#include <spectrum.h>
#include <string.h>
#include "../include/connect.h"
#include "../include/help.h"

extern char host_name[32];

/**
 * Connect to host prompt and input
 */
void connect(void)
{
  /* gotoxy(1,19); */
  /* cprintf("Press EXTEND 0 for help with PLATO keys."); */
  help_clear();
  help_prompt_input("Enter Hostname or <ENTER> for IRATA.ONLINE: ");
  cgets(host_name);
  if (strcmp(host_name,"")==0)
    {
      strcpy(host_name,"IRATA.ONLINE");
    }
  help_clear();
}
#endif

#ifdef __UNO__
#include "../include/uno/zifi.h"
#include "../include/help.h"
#include <conio.h>
#include <spectrum.h>
#include <string.h>

void connect() 
{
  char host[32];

  help_clear();
  help_prompt_input("Enter Hostname or <ENTER> for IRATA.ONLINE: ");
  cgets(host);
  if (strcmp(host,"")==0)
    {
      strcpy(host, "IRATA.ONLINE");
    }
  help_clear();

  initWifi();
  if (openTcp(&host, "8005")) {
    cprintf("Connected to host %s", &host);
    is_connected = 1;
  } else {
    cprintf("Failed to connect to host %s  ", &host);
    for (;;);
  }

}
#endif
