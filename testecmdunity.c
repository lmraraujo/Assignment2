#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cmdproc.h"
#include "unity.h"




void setUp(void) {
    // Initialize UART receive buffer
    memset(UARTRxBuff, 0, UART_RX_SIZE);
    rxBufflen = 0;

    // Initialize UART transmit buffer
    memset(UARTTxBuff, 0, UART_TX_SIZE);
    txBufflen = 0;

    // Initialize other variables if needed
    // Init sensors with mean value
   int temperature = (TEMP_MIN + TEMP_MAX) / 2;
    int humidity = (HUMIDITY_MIN + HUMIDITY_MAX) / 2;
   int o2 = (CO2_MIN + CO2_MAX) / 2;


}

void tearDown(void) {
    // Reset UART receive buffer
    memset(UARTRxBuff, 0, UART_RX_SIZE);
    rxBufflen = 0;

    // Reset UART transmit buffer
    memset(UARTTxBuff, 0, UART_TX_SIZE);
    txBufflen = 0;

    // Reset other variables if needed
   
    int temperature = (TEMP_MIN + TEMP_MAX) / 2;
    int humidity = (HUMIDITY_MIN + HUMIDITY_MAX) / 2;
    int co2 = (CO2_MIN + CO2_MAX) / 2;


}

void test_cmdProc_Invalid(void) {
    // Test cmdProc with a valid command
    // Example: '#At+023h042c12345!' should return SUCCESS
    unsigned char command[] = "#ft42090!";
    for (int i = 0; i < strlen(command); i++) {
        TEST_ASSERT_EQUAL_INT(SUCCESS, rxChar(command[i]));
    }
    TEST_ASSERT_EQUAL_INT(COMMAND_INVALID, cmdProc());
}

void test_cmdProc_Success(void) {
    // Test cmdProc with a valid command
    // Example: '#at+023h042c12345!' should return SUCCESS
    unsigned char command[] = "#pt12071!";
    for (int i = 0; i < strlen(command); i++) {
        TEST_ASSERT_EQUAL_INT(SUCCESS, rxChar(command[i]));
    }
    TEST_ASSERT_EQUAL_INT(SUCCESS, cmdProc());
}


void test_rxChar_Success(void) {
    // Test rxChar with a valid character
    TEST_ASSERT_EQUAL_INT(SUCCESS, rxChar('#'));
}

void test_txChar_Success(void) {
    // Test txChar with a valid character
    TEST_ASSERT_EQUAL_INT(SUCCESS, txChar('A'));
}

void test_resetRxBuff_Success(void) {
    // Test resetRxBuff
    resetRxBuff();
    TEST_ASSERT_EQUAL_INT(0, rxBufflen);
}

void test_resetTxBuff_Success(void) {
    // Test resetTxBuff
    resetTxBuff();
    TEST_ASSERT_EQUAL_INT(0, txBufflen);
}


void test_calcChecksum_Success(void) {
    // Test calcChecksum
    unsigned char buf[] = {'#', 'a', 't', '2', '2', '!'};
    int nbytes = sizeof(buf) / sizeof(buf[0]);
    TEST_ASSERT_EQUAL_INT(57, calcChecksum(buf, nbytes));
}





int main(void) {
    UNITY_BEGIN();
    // Run tests
    RUN_TEST(test_cmdProc_Success);
    RUN_TEST(test_rxChar_Success);
    RUN_TEST(test_txChar_Success);
    RUN_TEST(test_resetRxBuff_Success);
    RUN_TEST(test_resetTxBuff_Success);
    RUN_TEST(test_calcChecksum_Success);
    RUN_TEST(test_cmdProc_Invalid);
    UNITY_END();
}
