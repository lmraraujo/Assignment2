#ifndef CMD_PROC_H_
#define CMD_PROC_H

// Max size of the buffers
#define UART_RX_SIZE 20
#define UART_TX_SIZE 20
#define HISTORY 20

// Command constants
#define START_OF_FRAME '#'
#define END_OF_FRAME '!'

// Define error handling
#define SUCCESS 0
#define EMPTY_STRING -1 
#define COMMAND_INVALID -2
#define COMMAND_ERROR -3
#define STRING_ERROR -4
#define FULL_BUFFER -5
#define CHECKSUM_ERROR -6

// Supported commands
#define CMD_READ_ALL 'a'
#define CMD_READ_SENSOR 'p'
#define CMD_READ_LAST_SAMPLES 'l'
#define CMD_RESET_HISTORY 'r'

// Sensor data limits
#define TEMP_MIN -50
#define TEMP_MAX 60
#define HUMIDITY_MIN 0
#define HUMIDITY_MAX 100
#define CO2_MIN 400
#define CO2_MAX 20000

static unsigned char UARTTxBuff[UART_TX_SIZE];
static unsigned char UARTRxBuff[UART_RX_SIZE];
static unsigned char rxBufflen=0;
static unsigned char txBufflen=0;

int cmdProc(void);

int rxChar(unsigned char car);

int txChar(unsigned char car);

void resetRxBuff(void);

void resetTxBuff(void);

void getTxBuff(unsigned char *buf, int len);

int calcChecksum(unsigned char *buf, int nbytes);

int emulateSensors();

#endif