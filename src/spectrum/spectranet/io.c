#ifdef __SPECTRANET__
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sockpoll.h>
#include <netdb.h>
#include <spectrum.h>
#include "../../include/io.h"
#include "../../include/protocol.h"

static int sockfd, bytes, pfd;
static struct sockaddr_in remoteaddr;
char rxdata[1024];
struct hostent *he;
char host_name[32];

char io_initialized=0;
extern unsigned char is_extend;  //bring in is_extend for borders

void io_init(void)
{
  zx_border(INK_BLACK);
  he=gethostbyname(host_name);
  sockfd=socket(AF_INET,SOCK_STREAM,0);
  remoteaddr.sin_port=htons(8005);
  remoteaddr.sin_addr.s_addr=he->h_addr;
  connect(sockfd,&remoteaddr,sizeof(struct sockaddr_in));
  io_initialized=1;
}

void io_send_byte(unsigned char b)
{
  if (io_initialized==1)
    {
      send(sockfd,&b,sizeof(unsigned char), 0);
    }
}

void io_main(void)
{
  pfd=poll_fd(sockfd);
  if (pfd & POLLIN)
    {
      bytes=recv(sockfd,rxdata,1,0);
      ShowPLATO(rxdata,1);
    }
}
#endif /* __SPECTRANET__ */
