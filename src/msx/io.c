#ifdef __MSX__
#include <stdio.h>
#include <string.h>
#include <msx.h>
#include <arch/msx/asm.h>
#include "../include/io.h"
#include "../include/protocol.h"

static unsigned char inb;
static unapi_code_block codeBlock;
static Z80_registers regs;

enum TcpipUnapiFunctions {
    UNAPI_GET_INFO = 0,
    TCPIP_GET_CAPAB = 1,
    TCPIP_NET_STATE = 3,
    TCPIP_DNS_Q = 6,
    TCPIP_DNS_S = 7,
    TCPIP_TCP_OPEN = 13,
    TCPIP_TCP_CLOSE = 14,
    TCPIP_TCP_ABORT = 15,
    TCPIP_TCP_STATE = 16,
    TCPIP_TCP_SEND = 17,
    TCPIP_TCP_RCV = 18,
    TCPIP_WAIT = 29
};

enum TcpipErrorCodes {
    ERR_OK = 0,			    
    ERR_NOT_IMP,		
    ERR_NO_NETWORK,		
    ERR_NO_DATA,		
    ERR_INV_PARAM,		
    ERR_QUERY_EXISTS,	
    ERR_INV_IP,		    
    ERR_NO_DNS,		    
    ERR_DNS,		    
    ERR_NO_FREE_CONN,	
    ERR_CONN_EXISTS,	
    ERR_NO_CONN,		
    ERR_CONN_STATE,		
    ERR_BUFFER,		    
    ERR_LARGE_DGRAM,	
    ERR_INV_OPER
};

typedef struct {
  unsigned char remoteIP[4];
  unsigned int remotePort;
  unsigned int localPort;
  int userTimeout;
  unsigned char flags;
} TcpConnectionParameters;

static const char serverName="irata.online";

static TcpConnectionParameters conn_params;

const char* STRING_NO_UNAPI="NO UNAPI Implementation available.\r\n";
const char* STRING_NO_ACTIVE_TCP="No active TCP connections supported.\r\n";
const char* STRING_NO_NETWORK="No network available.\r\n";
const char* STRING_NO_DNS_SERVERS="No DNS servers configured.\r\n";
const char* STRING_DNS_SERVER_FAILURE="DNS Server Failure.\r\n";
const char* STRING_UNKNOWN_HOST_NAME="Unknown Host Name.\r\n";
const char* STRING_DNS_REFUSED="DNS Query Refused by server\r\n";
const char* STRING_DNS_NO_REPLY="No reply from DNS\r\n";
const char* STRING_DNS_QUERY_FAILED="DNS Query Failed.\r\n";
const char* STRING_CONNECTION_TIMEOUT="Connection timeout.\r\n";
const char* STRING_NO_CONNECTION_TO_SERVER="Could not establish connection to server\r\n";
const char* STRING_CONNECTED="Connection open\r\n";
const char* STRING_UNKNOWN_ERROR="Unknown Error\r\n";

static char temp[80];
static unsigned char conn;

#define LetTcpipBreathe() UnapiCall(&codeBlock, TCPIP_WAIT, &regs, REGS_NONE, REGS_NONE)

#define TICKS_TO_WAIT (20*60)
#define SYSTIMER ((unsigned int *)0xFC9E)

#define TCP_BUFFER_SIZE (4096)
#define TCPOUT_STEP_SIZE (512)

static int ticksWaited;
static int sysTimerHold;

char io_initialized=0;

void io_init(void)
{
  unsigned char count;

  // Check for UNAPI TCP/IP stack
  count=UnapiGetCount("TCP/IP");

  if (count==0)
    {
      ShowPLATO(STRING_NO_UNAPI,strlen(STRING_NO_UNAPI));
      return;
    }

  // Get the UNAPI code block
  UnapiBuildCodeBlock(NULL, 1, &codeBlock);

  // Check for ability to do active TCP connections.
  regs.Bytes.B = 1;
  UnapiCall(&codeBlock, TCPIP_GET_CAPAB, &regs, REGS_MAIN, REGS_MAIN);
  if((regs.Bytes.L & (1 << 3)) == 0)
    {
      ShowPLATO(STRING_NO_ACTIVE_TCP,strlen(STRING_NO_ACTIVE_TCP));
      return;
    }

  // Abort any active connections.
  regs.Bytes.B = 0;
  UnapiCall(&codeBlock, TCPIP_TCP_ABORT, &regs, REGS_MAIN, REGS_MAIN);

  // And set connection params we already know...
  conn_params.remotePort=8005;
  conn_params.localPort=0xffff;
  conn_params.userTimeout=0;
  conn_params.flags=0;

  // Check the network connection
  UnapiCall(&codeBlock, TCPIP_NET_STATE, &regs, REGS_NONE, REGS_MAIN);
  if(regs.Bytes.B == 0 || regs.Bytes.B == 3)
    {
      ShowPLATO(STRING_NO_NETWORK,strlen(STRING_NO_NETWORK));
      return;
    }
  
  // Resolve the Server name via DNS
  sprintf(temp,"Resolving %s ...\r\n",serverName);
  ShowPLATO(serverName,strlen(serverName));
  regs.Words.HL = (int)serverName;
  regs.Bytes.B = 0;
  UnapiCall(&codeBlock, TCPIP_DNS_Q, &regs, REGS_MAIN, REGS_MAIN);

  if(regs.Bytes.A == ERR_NO_DNS)
    {
      ShowPLATO(STRING_NO_DNS_SERVERS,strlen(STRING_NO_DNS_SERVERS));
      return;
    }
  else if(regs.Bytes.A != (unsigned char)ERR_OK)
    {
      /* sprintf(temp, "Unknown error when resolving the host name (code %i)", regs.Bytes.A); */
      /* ShowPLATO(temp,strlen(temp)); */
      ShowPLATO(STRING_UNKNOWN_ERROR,strlen(STRING_UNKNOWN_ERROR));
      return;
    }
  
  do
    {
      LetTcpipBreathe();
      regs.Bytes.B = 0;
      UnapiCall(&codeBlock, TCPIP_DNS_S, &regs, REGS_MAIN, REGS_MAIN);
    }
  while (regs.Bytes.A == 0 && regs.Bytes.B == 1);
  
  if(regs.Bytes.A != 0)
    {
      if(regs.Bytes.B == 2)
	{
	  ShowPLATO(STRING_DNS_SERVER_FAILURE,strlen(STRING_DNS_SERVER_FAILURE));
	  return;
	}
      else if(regs.Bytes.B == 3)
	{
	  ShowPLATO(STRING_UNKNOWN_HOST_NAME,strlen(STRING_UNKNOWN_HOST_NAME));
	  return;
	}
      else if(regs.Bytes.B == 5)
	{
	  ShowPLATO(STRING_DNS_REFUSED,strlen(STRING_DNS_REFUSED));
	  return;
	}
      else if(regs.Bytes.B == 16 || regs.Bytes.B == 17)
	{
	  ShowPLATO(STRING_DNS_NO_REPLY,strlen(STRING_DNS_NO_REPLY));
	  return;
	}    
      else if(regs.Bytes.B == 0)
	{
	  ShowPLATO(STRING_DNS_QUERY_FAILED,strlen(STRING_DNS_QUERY_FAILED));
	  return;
	}
      else
	{
	  sprintf(temp, "Unknown error returned by DNS server (code %i)", regs.Bytes.B);
	  ShowPLATO(temp,strlen(temp));
	  return;
	}
    }

  // We've successfully resolved, fill in the remote IP address.
  conn_params.remoteIP[0] = regs.Bytes.L;
  conn_params.remoteIP[1] = regs.Bytes.H;
  conn_params.remoteIP[2] = regs.Bytes.E;
  conn_params.remoteIP[3] = regs.Bytes.D;

  // Open TCP connection
  regs.Words.HL = (int)&(conn_params);
  UnapiCall(&codeBlock, TCPIP_TCP_OPEN, &regs, REGS_MAIN, REGS_MAIN);

  if(regs.Bytes.A == (unsigned char)ERR_NO_FREE_CONN)
    {
      regs.Bytes.B = 0;
      UnapiCall(&codeBlock, TCPIP_TCP_ABORT, &regs, REGS_MAIN, REGS_NONE);
      regs.Words.HL = (int)&(conn_params);
      UnapiCall(&codeBlock, TCPIP_TCP_OPEN, &regs, REGS_MAIN, REGS_MAIN);
    }
  
  if(regs.Bytes.A != 0)
    {
      sprintf(temp, "Unexpected error when opening TCP connection (%i)", regs.Bytes.A);
      ShowPLATO(temp,strlen(temp));
      return;
    }

  // A connection has been opened.
  conn = regs.Bytes.B;
    
    ticksWaited = 0;
    do
      {
        sysTimerHold = *SYSTIMER;
        LetTcpipBreathe();

	while(*SYSTIMER == sysTimerHold);

	ticksWaited++;

	if(ticksWaited >= TICKS_TO_WAIT)
	  {
	    ShowPLATO(STRING_CONNECTION_TIMEOUT,strlen(STRING_CONNECTION_TIMEOUT));
	  }
        regs.Bytes.B = conn;
        regs.Words.HL = 0;
        UnapiCall(&codeBlock, TCPIP_TCP_STATE, &regs, REGS_MAIN, REGS_MAIN);
      }
    while((regs.Bytes.A) == 0 && (regs.Bytes.B != 4));
    
    if(regs.Bytes.A != 0)
      {
	ShowPLATO(STRING_NO_CONNECTION_TO_SERVER,strlen(STRING_NO_CONNECTION_TO_SERVER));
	return;
      }
    else
      {
	ShowPLATO(STRING_CONNECTED,strlen(STRING_CONNECTED));
      }

  
}

void io_send_byte(unsigned char b)
{
}

void io_main(void)
{
}

#endif /* __MSX__ */

