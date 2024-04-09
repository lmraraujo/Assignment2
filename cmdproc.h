/**
 * @file cmdproc.h
 * @brief Header file for command processing module.
 *  @authors Luís Araújo, Bernardo Modesto
 * @date 9 Abril 2024
 */

#ifndef CMD_PROC_H_
#define CMD_PROC_H_


// Max size of the buffers
#define UART_RX_SIZE 20 /**< Maximum size of the UART receive buffer */
#define UART_TX_SIZE 20 /**< Maximum size of the UART transmit buffer */
#define HISTORY 20      /**< Maximum size of history data */

// Command constants
#define START_OF_FRAME '#' /**< Start of frame symbol */
#define END_OF_FRAME '!'   /**< End of frame symbol */

// Define error handling
#define SUCCESS 0          /**< Success indicator */
#define EMPTY_STRING -1    /**< Error code for empty string */
#define COMMAND_INVALID -2 /**< Error code for invalid command */
#define COMMAND_ERROR -3   /**< Error code for command processing error */
#define STRING_ERROR -4    /**< Error code for string processing error */
#define FULL_BUFFER -5     /**< Error code for full buffer */
#define CHECKSUM_ERROR -6  /**< Error code for checksum mismatch */
#define EMPTY_BUFFER -7    /**< Error code for empty buffer */

// Supported commands
#define CMD_READ_ALL 'a'            /**< Command to read all sensor values */
#define CMD_READ_SENSOR 'p'         /**< Command to read specific sensor value */
#define CMD_READ_LAST_SAMPLES 'l'   /**< Command to read last samples */
#define CMD_RESET_HISTORY 'r'       /**< Command to reset history */

// Sensor data limits
#define TEMP_MIN -50   /**< Minimum temperature value */
#define TEMP_MAX 60    /**< Maximum temperature value */
#define HUMIDITY_MIN 0 /**< Minimum humidity value */
#define HUMIDITY_MAX 100 /**< Maximum humidity value */
#define CO2_MIN 400    /**< Minimum CO2 value */
#define CO2_MAX 20000  /**< Maximum CO2 value */

static unsigned char UARTTxBuff[UART_TX_SIZE]; /**< UART transmit buffer */
static unsigned char UARTRxBuff[UART_RX_SIZE]; /**< UART receive buffer */
static unsigned char rxBufflen=0; /**< Length of the UART receive buffer */
static unsigned char txBufflen=0; /**< Length of the UART transmit buffer */

/**
 * @brief Process the received command.
 * @return Returns an error code indicating the status of command processing.
 */
int cmdProc(void);

/**
 * @brief Receive a character via UART.
 * @param car The character received via UART.
 * @return Returns an error code indicating the status of character reception.
 */
int rxChar(unsigned char car);

/**
 * @brief Transmit a character via UART.
 * @param car The character to transmit via UART.
 * @return Returns an error code indicating the status of character transmission.
 */
int txChar(unsigned char car);

/**
 * @brief Reset the UART receive buffer.
 */
void resetRxBuff(void);

/**
 * @brief Reset the UART transmit buffer.
 */
void resetTxBuff(void);

/**
 * @brief Get the UART transmit buffer.
 * @param buf Pointer to store the transmit buffer.
 * @param len Length of the transmit buffer.
 */
void getTxBuff(unsigned char *buf, int len);

/**
 * @brief Calculate the checksum of a buffer.
 * @param buf Pointer to the buffer.
 * @param nbytes Number of bytes in the buffer.
 * @return Returns the checksum value.
 */
int calcChecksum(unsigned char *buf, int nbytes);

/**
 * @brief Emulate sensor readings and process received commands.
 * @return Returns an error code indicating the status of emulation and command processing.
 */
int emulateSensors();

#endif
