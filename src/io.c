#include <stdio.h>
#include <string.h>
#ifdef __RS232__
#include <rs232.h>
#endif
#ifdef __SPECTRANET__
#include <sys/socket.h>
#include <sys/types.h>
#include <sockpoll.h>
#include <netdb.h>
#endif
#ifdef __SPECTRUM__
#include <spectrum.h>
#endif
#include "io.h"
#include "protocol.h"

#ifdef __RS232__
static unsigned char inb;
#endif
#ifdef __SPECTRANET__
static int sockfd, bytes, pfd;
static struct sockaddr_in remoteaddr;
char rxdata[1024];
struct hostent *he;
char host_name[32];
#endif

char io_initialized=0;

void io_init(void)
{
#ifdef __RS232__
  rs232_params(RS_BAUD_4800|RS_STOP_1|RS_BITS_8,RS_PAR_NONE);  //  Bauds tested 1200[/] 2400[/] 4800[/] 9600[] 19200[] 38400[] 57600[] 115200[] 
  rs232_init();
#endif
#ifdef __SPECTRANET__
  zx_border(INK_BLACK);
  he=gethostbyname(host_name);
  sockfd=socket(AF_INET,SOCK_STREAM,0);
  remoteaddr.sin_port=htons(8005);
  remoteaddr.sin_addr.s_addr=he->h_addr;
  connect(sockfd,&remoteaddr,sizeof(struct sockaddr_in));
#endif
  io_initialized=1;

}

void io_init_funcptrs(void)
{
}

void io_open(void)
{  
}

void io_send_byte(unsigned char b)
{
#ifdef __RS232__
  if (io_initialized==1)
    {
	  //zx_border(INK_RED);  //DEBUG Timing *IRQ-OFF
      rs232_put(b);
	  //zx_border(INK_CYAN);  //DEBUG Timing *IRQ-ON
    }
#endif
#ifdef __SPECTRANET__
  if (io_initialized==1)
    {
      send(sockfd,&b,sizeof(unsigned char), 0);
    }
#endif
}

void io_main(void)
{
#ifdef __RS232__										
	//zx_border(INK_RED);  //DEBUG Timing *IRQ-OFF		// <TIME>*IRQ STATE [EXECUTION PATH]
  if (rs232_get(&inb) != RS_ERR_NO_DATA)  				// Bit bashing port Path:[DATA<80*IRQ-OFF>] || [NO DATA<10>*IRQ-OFF] 	// *IRQ - OFF, getK() blocked
    {//zx_border(INK_CYAN);  //DEBUG Timing *IRQ-ON
	  /* [RX TTY - Draw Screen] */  																	 		
      ShowPLATO(&inb,1);  // Draw terminal <20>*IRQ-ON																// *IRQ - ON
    }
  else
    {  /* [NO RX TTY - KEY scan] */  												 		
	  for(int Kscan=0;Kscan<40;Kscan++)																				// *IRQ - ON/OFF(IF Key TTY)
		{
			keyboard_main(); // [Path: [No key<1>*IRQ-ON] || [key local process<2>*IRQ-ON] || [key send data<80>*IRQ-OFF]
		}
    }
#endif
#ifdef __SPECTRANET__
  pfd=poll_fd(sockfd);
  if (pfd & POLLIN)
    {
      bytes=recv(sockfd,rxdata,1,0);
      ShowPLATO(rxdata,1);
    }
#endif
}

void io_recv_serial(void)
{  
}

void io_done()
{
}
