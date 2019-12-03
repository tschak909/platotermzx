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
#include "../../include/keyboard.h"

static int sockfd, bytes, pfd;
static struct sockaddr_in remoteaddr;
char rxdata[2049];
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
      bytes=recv(sockfd,rxdata,2048,0);
      ShowPLATO(rxdata,bytes);
    }
  else
    {
/*  TEST 1
      in_Pause(1);
      keyboard_main();
*/

//  BEST
      for(int Kscan=0;Kscan<10;Kscan++)  //Extra keyboard scanning					
      {
        in_Pause(1);
        keyboard_main();
      } 


/* ORIGINAL
      for(int Kscan=0;Kscan<30;Kscan++)  //Extra keyboard scanning					
      {
        keyboard_main();
      } 
*/
    }
  
}
#endif /* __SPECTRANET__ */
