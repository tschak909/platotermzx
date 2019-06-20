#ifndef ZIFI_H
#define ZIFI_H 1

#define UART_DATA_REG 0xc6
#define UART_STAT_REG  0xc7
#define UART_BYTE_RECIVED 0x80
#define UART_BYTE_SENT 0x40
#define ZXUNO_ADDR 0xFC3B
#define ZXUNO_REG 0xFD3B

// ESX API
#define ESX_GETSETDRV 0x89
#define ESX_FOPEN 0x9A
#define ESX_FCLOSE 0x9B
#define ESX_FSYNC 0x9C
#define ESX_FREAD 0x9D
#define ESX_FWRITE 0x9E

// File Modes
#define FMODE_READ 0x01
#define FMODE_WRITE 0x06
#define FMODE_CREATE 0x0E

extern char* ssid;
extern char is_connected;

void initWifi();
char openTcp(char *host, char *port);
char isAvail();
char sendByte(char c);
char getByte();

#endif