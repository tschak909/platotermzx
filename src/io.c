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

#ifdef __UNO__
#include "uno/zifi.h"

static unsigned char inb;
#endif 

char io_initialized=0;
extern unsigned char is_extend;  //bring in is_extend for borders

void io_init(void)
{
#ifdef __RS232__
  rs232_params(RS_BAUD_9600|RS_STOP_1|RS_BITS_8,RS_PAR_NONE);  //  Bauds tested 1200[/] 2400[/] 4800[/] 9600[/] 19200[X] 38400[X] 57600[] 115200[] 
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
#ifdef __SPECTRUM__
      if(is_extend==1) {zx_border(INK_BLACK);}  else {zx_border(INK_WHITE);}	//RS232 Raster Bars
#endif
      rs232_put(b);	//*IRQ-OFF (SENDING DATA)

#ifdef __SPECTRUM__
      if(is_extend==1) {zx_border(INK_GREEN);}  else {zx_border(INK_BLACK);}	//RS232 Raster Bars
#endif
    }
#endif
#ifdef __SPECTRANET__
  if (io_initialized==1)
    {
      send(sockfd,&b,sizeof(unsigned char), 0);
    }
#endif

#ifdef __UNO__
  if (io_initialized==1) {
    sendByte(b);
  }
#endif
}

void io_main(void)
{
#ifdef __RS232__

	//Don't try to wrap this in for Rasta bars, it just flashes every call to io_main.										
  if (rs232_get(&inb) != RS_ERR_NO_DATA)  	// *IRQ-OFF (RECEIVING DATA)
    {	/* [RX - Display] */ 	
#ifdef __SPECTRUM__
        if(is_extend==1) {zx_border(INK_BLACK);}  else {zx_border(INK_WHITE);}	//RS232 Raster Bars- A little lie, the IO has been done.
#endif
	ShowPLATO(&inb,1);
#ifdef __SPECTRUM__
	if(is_extend==1) {zx_border(INK_GREEN);}  else {zx_border(INK_BLACK);}	//RS232 Raster Bars			
#endif
    }
  else
    {  /* [NO RX - KEY scan] */  
#ifdef __SPECTRUM__
	if(is_extend==1) {zx_border(INK_GREEN);}  else {zx_border(INK_BLACK);}	//RS232 Raster Bars
#endif										 		
	for(int Kscan=0;Kscan<30;Kscan++)  //Extra keyboard scanning					
	  {
	    keyboard_main();
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

#ifdef __UNO__
  while (isAvail()) {
      inb = getByte();
      ShowPLATO(&inb,1);
	    }
#endif 
}

void io_recv_serial(void)
{  
}

void io_done()
{
}
