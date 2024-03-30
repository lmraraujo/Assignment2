#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmdproc.h"

static unsigned char UARTRxBuff[UART_RX_SIZE];
static unsigned char rxBufflen=0;

static unsigned char UARTTxBuff[UART_TX_SIZE];
static unsigned char txBufflen=0;

int temphist[HISTORY];
int humhist[HISTORY];
int cohist[HISTORY];
int histIndex = 0;

// Init sensors with mean value
int temperature = (TEMP_MIN + TEMP_MAX) / 2;
int humidity = (HUMIDITY_MIN + HUMIDITY_MAX) / 2;
int co2 = (CO2_MIN + CO2_MAX) / 2;

int cmdProc(void)
{
    int i;
    int counth=0;
    int countc=0;
    int countt=0;
    unsigned char sid;  //ID do sensor se é hum temp ou co2

    if (rxBufflen == 0) return EMPTY_STRING;

    for (i=0; i<rxBufflen; i++){
        if(UARTRxBuff[i] == START_OF_FRAME) break;
    }

    if (i < rxBufflen){

        switch(UARTRxBuff[i+1]){

            case CMD_READ_SENSOR:    
                sid = UARTRxBuff[i+2];
                
                if (sid != 't' && sid != 'h' && sid != 'c'){
                    return COMMAND_INVALID;
                }  
             
                if(!(calcChecksum(&(UARTRxBuff[i+1]),2))) {
					return COMMAND_ERROR;
				}

                if(UARTRxBuff[i+6] != EOF_SYM) {
					return STRING_ERROR;
				}
                
                emulateSensors(); // emulate values from the sensor
                
                

                int l=0;
                int p;

                if(sid == 't'){
                   for(int l=0; l<txBufflen;l++){
                      
                    if (UARTTxBuff[l] == 'h'||counth>0||counth<4){
                        UARTTxBuff[l] ='\0';
                        counth++;
                        if(counth==4){
                            l=l-4;
                            p=l;
                            while(p<txBufflen-4){

                                UARTTxBuff[p]=UARTTxBuff[p+4];
                                p++;
                            }
                        }
                    }
                    if (UARTTxBuff[l] == 'c'||countc>0||countc<6){
                        UARTTxBuff[l] ='\0';
                        countc++;
                        if(countc==6){
                            l=l-6;
                            p=l;
                            while(p<txBufflen-6){

                                UARTTxBuff[p]=UARTTxBuff[p+6];
                                p++;
                            }
                            while(p<txBufflen){
                                UARTTxBuff[p]='\0';
                                p++;
                            }
                        }
                    }

                   }
                }




                if(sid == 'h'){

                   for(int l=0; l<txBufflen;l++){
                      
                    if (UARTTxBuff[l] == 't'||countt>0||countt<4){
                        UARTTxBuff[l] ='\0';
                        countt++;
                        if(countt==4){
                            l=l-4;
                            p=l;
                            while(p<txBufflen-4){

                                UARTTxBuff[p]=UARTTxBuff[p+4];
                                p++;
                            }
                        }
                    }
                    if (UARTTxBuff[l] == 'c'||countc>0||countc<6){
                        UARTTxBuff[l] ='\0';
                        countc++;
                        if(countc==6){
                            l=l-6;
                            p=l;
                            while(p<txBufflen-6){

                                UARTTxBuff[p]=UARTTxBuff[p+6];
                                p++;
                            }
                            while(p<txBufflen){
                                UARTTxBuff[p]='\0';
                                p++;
                            }
                        }
                    }

                   }

                    
                }





                if(sid == 'c'){
                    for(int l=0; l<txBufflen;l++){
                      
                    if (UARTTxBuff[l] == 't'||countt>0||countt<4){
                        UARTTxBuff[l] ='\0';
                        countt++;
                        if(countt==4){
                            l=l-4;
                            p=l;
                            while(p<txBufflen-4){

                                UARTTxBuff[p]=UARTTxBuff[p+4];
                                p++;
                            }
                        }
                    }
                    if (UARTTxBuff[l] == 'h'||counth>0||counth<4){
                        UARTTxBuff[l] ='\0';
                        counth++;
                        if(counth==4){
                            l=l-4;
                            p=l;
                            while(p<txBufflen-4){

                                UARTTxBuff[p]=UARTTxBuff[p+4];
                                p++;
                            }
                            while(p<txBufflen){
                                UARTTxBuff[p]='\0';
                                p++;
                            }
                        }
                    }

                    
                }

                return SUCCESS;  

            case CMD_READ_ALL:
                
                emulateSensors();

                return SUCCESS;  

            case CMD_READ_LAST_SAMPLES:
                
                emulatesensors();

                //ir buscar os arrays de historico

                return SUCCESS;  

            case CMD_RESET_HISTORY:
                
                for(int index=0;index<HISTORY;index++){
                    temphist[index] = '\0';
                    humhist[index] = '\0';
                    cohist[index] = '\0';
                    histIndex=0;
                }

                return SUCCESS;    

            default:
                return COMMAND_INVALID;




        }

          }
    
    }
    return STRING_ERROR;
}


int rxChar(unsigned char car)
{
	/* If rxbuff not full add char to it */
	if (rxBufflen < UART_RX_SIZE) {
		UARTRxBuff[rxBufflen] = car;
		rxBufflen += 1;
		return SUCCESS;		
	}	
	/* If cmd string full return error */
	return STRING_ERROR;//MUDAR
}

int txChar(unsigned char car)
{
	/* If rxbuff not full add char to it */
	if (txBufflen < UART_TX_SIZE) {
		UARTTxBuff[txBufflen] = car;
		txBufflen += 1;
		return 0;		
	}	
	/* If cmd string full return error */
	return STRING_ERROR;//MUDAR
}

void resetRxBuff(void)
{
    // Reset each element of the RX buffer to '\0' (null character)
    for (int i = 0; i < UART_RX_SIZE; i++) {
        UARTRxBuff[i] = '\0';
    }
    // Reset the length to zero
    rxBufflen = 0;		
}

void resetTxBuff(void)
{
	// Reset each element of the TX buffer to '\0' (null character)
    for (int i = 0; i < UART_TX_SIZE; i++) {
        UARTTxBuff[i] = '\0';
    }
    // Reset the length to zero
    txBufflen = 0;		
}

void getTxBuff(unsigned char * buf, int * len)
{
    // Copy the length of the TX buffer to the variable pointed to by len
    *len = txBufflen;
    
    // Check if the TX buffer contains any data
    if (txBufflen > 0) {
        // If the buffer is not empty, copy its contents to the buffer pointed to by buf
        memcpy(buf, UARTTxBuff, *len);
    }		
}

int emulateSensors(){
    int i=0;
    // Adiciona um pequeno valor aleatório aos valores dos sensores
    temperature += (rand() % 3) - 1; // variação de -1 a 1
    humidity += (rand() % 3) - 1; // variação de -1 a 1
    co2 += (rand() % 3) - 1; // variação de -1 a 1

    // Garante que os valores dos sensores estão dentro dos limites
    if (temperature < TEMP_MIN) temperature = TEMP_MIN;
    if (temperature > TEMP_MAX) temperature = TEMP_MAX;
    if (humidity < HUMIDITY_MIN) humidity = HUMIDITY_MIN;
    if (humidity > HUMIDITY_MAX) humidity = HUMIDITY_MAX;
    if (co2 < CO2_MIN) co2 = CO2_MIN;
    if (co2 > CO2_MAX) co2 = CO2_MAX;

    // History handling
 
    if(histIndex>19){
        histIndex=0;
        //nao vao estar por ordem apos completar o array pela primeira vez
    }

    temphist[histIndex] = temperature;
    humhist[histIndex] = humidity;
    cohist[histIndex] = co2;

    histIndex++;



   // Convert sensor data to characters and store them in the UARTTxBuff
    int len = 0;
    resetTxBuff(); // Reset buffer
    txChar('#');   // Start of frame
    len++;         // Increment length

    if(UARTRxBuff[i+1] == 'p'){
        txChar('p');
        len++;
    }
    if(UARTRxBuff[i+1] == 'a'){
        txChar('a');
        len++;
    }
    if(UARTRxBuff[i+1] == 'l'){
        txChar('l');
        len++;
    }
    if(UARTRxBuff[i+1] == 'r'){
        txChar('r');
        len++;
    }

    txChar('t');
    len++;

    // Convert and transmit the sign for temperature
    if (temperature >= 0) {
        txChar('+');
    } else {
        txChar('-');
        temperature = -temperature; // Make temperature positive for further processing
    }
    len++; // Increment length

    // Transmit temperature value
    txChar('0' + (temperature / 10)); // Tens digit
    len++;
    txChar('0' + (temperature % 10)); // Units digit
    len++;

    txChar('h');
    len++;

    // Convert and transmit humidity
    txChar('0' + (humidity / 100)); // Hundreds digit
    len++;
    txChar('0' + ((humidity / 10) % 10)); // Tens digit
    len++;
    txChar('0' + (humidity % 10)); // Units digit
    len++;

    txChar('c');
    len++;

    // Convert and transmit CO2 level
    txChar('0' + (co2 / 10000)); // Ten-thousands digit
    len++;
    txChar('0' + ((co2 / 1000) % 10)); // Thousands digit
    len++;
    txChar('0' + ((co2 / 100) % 10)); // Hundreds digit
    len++;
    txChar('0' + ((co2 / 10) % 10)); // Tens digit
    len++;
    txChar('0' + (co2 % 10)); // Units digit
    len++;

    int lencheck=len;

    txChar('!'); // End of frame        
    len++;       // Increment length
    
    int checkin = calcChecksum(UARTTxBuff, lencheck);

    int checkot = calcChecksum(UARTTxBuff, lencheck);
    if (checkin==checkot){
       
        UARTTxBuff[len]='0' + (checkot / 100)//tinha aqui len1 ja ns oq era
        txChar('0' + ((checkot / 10) % 10)); // Tens digit
        len++;
        txChar('0' + (checkot % 10)); // Units digit
        len++;
        txChar('!'); // End of frame        
        len++;       // Increment length
        // If the buffer is not full, fill the remaining spaces with null characters
        for (int k = len; i < UART_TX_SIZE; k++) {
        txChar('\0');
        }
        checkot = calcChecksum(UARTTxBuff, lencheck);
        if (checkin==checkot){
           return SUCCESS;
        }
        else return CHECKSUM_ERROR;
        
    }
    else{
        return CHECKSUM_ERROR;
    }

}
int calcChecksum(unsigned char *buf, int nbytes) {
     // Compute the checksum as the sum of the numerical value of each byte in the buffer
    int checksum = 0;
    for (int i = 1; i < nbytes; i++) {
        
        checksum += buf[i];
    }

    // Compute the modulo 256 checksum
    checksum %= 256;

    // Return the checksum value
    return checksum;
}