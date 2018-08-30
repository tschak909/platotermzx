#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sockpoll.h>
#include <netdb.h>
#include "io.h"
#include "protocol.h"

static int sockfd, bytes;
static struct sockaddr_in remoteaddr;
char rxdata[1024];
struct hostent *he;

void io_init(void)
{
  he=gethostbyname("irata.online");
  sockfd=socket(AF_INET,SOCK_STREAM,0);
  remoteaddr.sin_port=htons(8005);
  remoteaddr.sin_addr.s_addr=he->h_addr;
  connect(sockfd,&remoteaddr,sizeof(sockaddr_in));
}

void io_init_funcptrs(void)
{
}

void io_open(void)
{  
}

void io_send_byte(unsigned char b)
{
  send(sockfd,b,sizeof(unsigned char), 0);
}

void io_main(void)
{
  if (poll_fd(sockfd) == POLLIN)
    {
      bytes=recv(sockfd,rxdata,sizeof(rxdata)-1,0);
      ShowPLATO(rxdata,bytes);
    }
}

void io_recv_serial(void)
{  
}

void io_done()
{
}
