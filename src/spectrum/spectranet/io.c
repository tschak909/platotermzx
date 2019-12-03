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
      // RX data mark
      //gotoxy(0,0);    printf("X");
      bytes=recv(sockfd,rxdata,2048,0);
      // Clear the RX data mark
      //gotoxy(0,0);    printf(" ");

      if(bytes!=0)  // Seems to stop some bugs with the network stopping
        ShowPLATO(rxdata,bytes);
    }
  else
    {

      in_Pause(10);  //This time drops to 0 on key push
      keyboard_main();


//  This is another way to do it and get more Keys sent before RX
/*
      for(int Kscan=0;Kscan<10;Kscan++)  //Extra keyboard scanning					
      {// THIS IS THE MAIN KEYBOARD SCAN WINDOW NOW  
        in_Pause(1);
        keyboard_main();
      } 
*/

//  ORIGINAL  -- Served us well but not required I have grown
/*
      for(int Kscan=0;Kscan<30;Kscan++)  //Extra keyboard scanning					
      {
        keyboard_main();
      } 
*/
    }
  
}
#endif /* __SPECTRANET__ */
