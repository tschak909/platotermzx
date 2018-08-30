#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sockpoll.h>
#include <netdb.h>
#include <spectrum.h>
#include "io.h"
#include "protocol.h"

static int sockfd, bytes, pfd;
static struct sockaddr_in remoteaddr;
char rxdata[1024];
struct hostent *he;
char io_initialized=0;

void io_init(void)
{
  zx_border(INK_BLACK);
  he=gethostbyname("irata.online");
  sockfd=socket(AF_INET,SOCK_STREAM,0);
  remoteaddr.sin_port=htons(8005);
  remoteaddr.sin_addr.s_addr=he->h_addr;
  connect(sockfd,&remoteaddr,sizeof(struct sockaddr_in));
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
  if (io_initialized==1)
    {
      zx_border(INK_CYAN);
      send(sockfd,b,sizeof(unsigned char), 0);
    }
}

void io_main(void)
{
  pfd=poll_fd(sockfd);
  if (pfd & POLLIN)
    {
      zx_border(INK_WHITE);
      bytes=recv(sockfd,rxdata,1,0);
      ShowPLATO(rxdata,1);
    }
  else if (pfd & POLLCON)
    {
      zx_border(INK_BLUE);
    }
  else if (pfd & POLLHUP)
    {
      zx_border(INK_RED);
    }
  else if (pfd & POLLNVAL)
    {
      zx_border(INK_YELLOW);
    }
  else
    {
      zx_border(INK_GREEN);
    }
}

void io_recv_serial(void)
{  
}

void io_done()
{
}
