#include <stdio.h>
#include <string.h>
#ifdef __RS232__
#include <rs232.h>
#else
#include <sys/socket.h>
#include <sys/types.h>
#include <sockpoll.h>
#include <netdb.h>
#endif
#include <spectrum.h>
#include "io.h"
#include "protocol.h"

#ifdef __RS232__
static unsigned char inb;
#else
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
  rs232_params(RS_BAUD_1200|RS_STOP_1|RS_BITS_8,RS_PAR_NONE);
  rs232_init();
#else
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
      rs232_put(b);
    }
#else
  if (io_initialized==1)
    {
      send(sockfd,&b,sizeof(unsigned char), 0);
    }
#endif
}

void io_main(void)
{
#ifdef __RS232__
  if (rs232_get(&inb) != RS_ERR_NO_DATA)
    {
      ShowPLATO(&inb,1);
    }
#else
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
