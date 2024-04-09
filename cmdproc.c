#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cmdproc.h"




int temphist[HISTORY];
int humhist[HISTORY];
int cohist[HISTORY];
int histIndex = 0;

static unsigned char auxBuff[20];

char string[5];
char array [4];

// Init sensors with mean value
int temperature = (TEMP_MIN + TEMP_MAX) / 2; //5 A-15,V-13,G-11,Al-18
int humidity = (HUMIDITY_MIN + HUMIDITY_MAX) / 2;//50 A-60, V-40, G-55, Al-35
int co2 = (CO2_MIN + CO2_MAX) / 2;//10200 A-500, V-300, G-400, Al-450

int cmdProc(void)
{
    int i;
    int counth=0;
    int countc=0;
    int countt=0;
    unsigned char sid;  //ID do sensor se é hum temp ou co2
    unsigned char processing;

    if (rxBufflen == 0) return EMPTY_STRING;

    
    if(UARTRxBuff[0] != START_OF_FRAME) return COMMAND_INVALID;
        
    

    temperature = (TEMP_MIN + TEMP_MAX) / 2; //5 A-15,V-13,G-11,Al-18
    humidity = (HUMIDITY_MIN + HUMIDITY_MAX) / 2;//50 A-60, V-40, G-55, Al-35
    co2 = (CO2_MIN + CO2_MAX) / 2;//10200 A-500, V-300, G-400, Al-450

    if (i < rxBufflen){
        unsigned char regiao= UARTRxBuff[i+3];
        switch(regiao){
                    case '1'://aveiro
                        temperature+=10;
                        humidity+=10;
                        co2-=9700;
                        break;
                    case '2'://viseu
                        temperature+=8;
                        humidity-=10;
                        co2-=9900;
                        break;
                    case '3'://guarda
                        temperature+=6;
                        humidity+=5;
                        co2-=9800;
                        break;
                    case '4'://algarve
                        temperature+=13;
                        humidity-=15;
                        co2-=9750;
                        break;
                }
                

        //CS de entrada
        int check_entrada=0;

        array[0]=UARTRxBuff[5];
        array[1]=UARTRxBuff[6];
        array[2]=UARTRxBuff[7];

        memcpy(string,array,3);
        string[3]='\0';

        check_entrada+=atoi(string);

        printf("%d",check_entrada);

        if(calcChecksum(UARTRxBuff,5)==check_entrada)printf("\nChecksum correto,(transmissao bem sucedida)\n");
        else{ 

            
            return -8;
        }
        
        switch(UARTRxBuff[i+1]){

            case CMD_READ_SENSOR:    
                sid = UARTRxBuff[i+2];
                
                if (sid != 't' && sid != 'h' && sid != 'c'){
                    return COMMAND_INVALID;
                }  
             
                if(!(calcChecksum(&(UARTRxBuff[i+1]),2))) {
					return COMMAND_ERROR;
				}

                if(UARTRxBuff[i+8] != END_OF_FRAME) {
					return STRING_ERROR;
				}
             


                emulateSensors(); // emulate values from the sensor
                
                

                int l=0;
                int p=0;
                //for(l=0;l<UART_TX_SIZE;l++){
                //printf("%c ",UARTTxBuff[l]);}

                if(sid == 't'){

                    for(l=0;l<UART_TX_SIZE;l++){
                        if((counth>0 && counth<4)|| UARTTxBuff[l]=='h'){
                            counth++;
                        }
                        else if((countc>0 && countc<6)|| UARTTxBuff[l]=='c'){
                            countc++;
                        }
                        else{
                            auxBuff[p]= UARTTxBuff[l];
                            p++;
                        }
                    }
                    resetTxBuff();
                    
                    processing= UARTRxBuff[i+4];
                    if(processing=='1'){
                        for(l=0;l<UART_TX_SIZE;l++){
                            printf("%c ",auxBuff[l]);
                            }

                    }
                    else if(processing=='2'){
                        if(regiao=='1'){
                            printf("\nO sensor presente em Aveiro enviou os seguintes dados:\n");
                            if(sid=='t'){
                                printf("Temperatura: %c%c%c\n",auxBuff[3], auxBuff[4], auxBuff[5]);
                            }
                            printf("Checksum: %c%c%c\n",auxBuff[6], auxBuff[7], auxBuff[8]);
                        }
                        if(regiao=='2'){
                            printf("\nO sensor presente em Viseu enviou os seguintes dados:\n");
                            if(sid=='t'){
                                printf("Temperatura: %c%c%c\n",auxBuff[3], auxBuff[4], auxBuff[5]);
                            }
                            printf("Checksum: %c%c%c\n",auxBuff[6], auxBuff[7], auxBuff[8]);
                        }
                        if(regiao=='3'){
                            printf("\nO sensor presente na Guarda enviou os seguintes dados:\n");
                            if(sid=='t'){
                                printf("Temperatura: %c%c%c\n",auxBuff[3], auxBuff[4], auxBuff[5]);
                            }
                            printf("Checksum: %c%c%c\n",auxBuff[6], auxBuff[7], auxBuff[8]);
                        }
                        if(regiao=='4'){
                            printf("\nO sensor presente no Algarve enviou os seguintes dados:\n");
                            if(sid=='t'){
                                printf("Temperatura: %c%c%c\n",auxBuff[3], auxBuff[4], auxBuff[5]);
                            }
                            printf("Checksum: %c%c%c\n",auxBuff[6], auxBuff[7], auxBuff[8]);
                        }
                    }
                    

                }



                p=0;
                if(sid == 'h'){

                  for(l=0;l<UART_TX_SIZE;l++){
                        if((countt>0 && countt<4)|| UARTTxBuff[l]=='t'){
                            countt++;
                        }
                        else if((countc>0 && countc<6)|| UARTTxBuff[l]=='c'){
                            countc++;
                        }
                        else{
                            auxBuff[p]= UARTTxBuff[l];
                            p++;
                        }
                    }
                    resetTxBuff();
                    processing= UARTRxBuff[i+4];
                    if(processing=='1'){
                        for(l=0;l<UART_TX_SIZE;l++){
                            printf("%c ",auxBuff[l]);
                            }

                    }
                    else if(processing=='2'){
                        if(regiao=='1'){
                            printf("\nO sensor presente em Aveiro enviou os seguintes dados:\n");
                            if(sid=='h'){
                                printf("Humidade: %c%c%c\n",auxBuff[3], auxBuff[4], auxBuff[5]);
                            }
                            printf("Checksum: %c%c%c\n",auxBuff[6], auxBuff[7], auxBuff[8]);
                        }
                        if(regiao=='2'){
                            printf("\nO sensor presente em Viseu enviou os seguintes dados:\n");
                            if(sid=='h'){
                                printf("Humidade: %c%c%c\n",auxBuff[3], auxBuff[4], auxBuff[5]);
                            }
                            printf("Checksum: %c%c%c\n",auxBuff[6], auxBuff[7], auxBuff[8]);
                        }
                        if(regiao=='3'){
                            printf("\nO sensor presente na Guarda enviou os seguintes dados:\n");
                            if(sid=='h'){
                                printf("Humidade: %c%c%c\n",auxBuff[3], auxBuff[4], auxBuff[5]);
                            }
                            printf("Checksum: %c%c%c\n",auxBuff[6], auxBuff[7], auxBuff[8]);
                        }
                        if(regiao=='4'){
                            printf("\nO sensor presente no Algarve enviou os seguintes dados:\n");
                            if(sid=='h'){
                                printf("Humidade: %c%c%c\n",auxBuff[3], auxBuff[4], auxBuff[5]);
                            }
                            printf("Checksum: %c%c%c\n",auxBuff[6], auxBuff[7], auxBuff[8]);
                        }
                    }
                    


                    
                }




                p=0;
                if(sid == 'c'){
                    for(l=0;l<UART_TX_SIZE;l++){
                        if((counth>0 && counth<4)|| UARTTxBuff[l]=='t'){
                            counth++;
                        }
                        else if((countc>0 && countc<4)|| UARTTxBuff[l]=='h'){
                            countc++;
                        }
                        else{
                            auxBuff[p]= UARTTxBuff[l];
                            p++;
                        }
                    }
                    resetTxBuff();
                    processing= UARTRxBuff[i+4];
                    if(processing=='1'){
                        for(l=0;l<UART_TX_SIZE;l++){
                            printf("%c ",auxBuff[l]);
                            }

                    }
                    else if(processing=='2'){
                        if(regiao=='1'){
                            printf("\nO sensor presente em Aveiro enviou os seguintes dados:\n");
                            if(sid=='c'){
                                printf("Co2: %c%c%c%c%c\n",auxBuff[3], auxBuff[4], auxBuff[5], auxBuff[6], auxBuff[7]);
                            }
                            printf("Checksum: %c%c%c\n",auxBuff[8], auxBuff[9], auxBuff[10]);
                        }
                        if(regiao=='2'){
                            printf("\nO sensor presente em Viseu enviou os seguintes dados:\n");
                            if(sid=='c'){
                                printf("Co2: %c%c%c%c%c\n",auxBuff[3], auxBuff[4], auxBuff[5], auxBuff[6], auxBuff[7]);
                            }
                            printf("Checksum: %c%c%c\n",auxBuff[8], auxBuff[9], auxBuff[10]);
                        }
                        if(regiao=='3'){
                            printf("\nO sensor presente na Guarda enviou os seguintes dados:\n");
                            if(sid=='c'){
                                printf("Co2: %c%c%c%c%c\n",auxBuff[3], auxBuff[4], auxBuff[5], auxBuff[6], auxBuff[7]);
                            }
                            printf("Checksum: %c%c%c\n",auxBuff[8], auxBuff[9], auxBuff[10]);
                        }
                        if(regiao=='4'){
                            printf("\nO sensor presente no Algarve enviou os seguintes dados:\n");
                            if(sid=='c'){
                               printf("Co2: %c%c%c%c%c\n",auxBuff[3], auxBuff[4], auxBuff[5], auxBuff[6], auxBuff[7]);
                            }
                            printf("Checksum: %c%c%c\n",auxBuff[6], auxBuff[7], auxBuff[8]);
                        }
                    }
                    
                }
                return SUCCESS;  

            case CMD_READ_ALL:
                
                emulateSensors();
                processing= UARTRxBuff[i+4];
                if(processing == '1'){
                    for(l=0;l<UART_TX_SIZE;l++){
                        printf("%c ",UARTTxBuff[l]);
                        }
                }
                else if(processing == '2'){
                        if(regiao=='1'){
                            printf("\nO sensor presente em Aveiro enviou os seguintes dados:\n");
                        }
                        if(regiao=='2'){
                            printf("\nO sensor presente em Viseu enviou os seguintes dados:\n");
                           
                        }
                        if(regiao=='3'){
                            printf("\nO sensor presente na Guarda enviou os seguintes dados:\n");
                        }
                        if(regiao=='4'){
                            printf("\nO sensor presente no Algarve enviou os seguintes dados:\n");
                        }
                        printf("Temperatura: %c%c%c\n",UARTTxBuff[3],UARTTxBuff[4],UARTTxBuff[5]);
                        printf("Humidade: %c%c%c\n",UARTTxBuff[7],UARTTxBuff[8],UARTTxBuff[9]);
                        printf("Co2: %c%c%c%c%c\n",UARTTxBuff[11],UARTTxBuff[12],UARTTxBuff[13],UARTTxBuff[14],UARTTxBuff[15]);

                }
                return -9;  

            case CMD_READ_LAST_SAMPLES:
                
                
                int printIndex;

                    if(histIndex!=0){
                        printf("\nTemperatura: ");
                        for(printIndex=0;printIndex<HISTORY;printIndex++){
                            if(temphist[printIndex]!=0) printf("%d ",temphist[printIndex]);
                        }
                        printf("\nHumidade: ");
                        for(printIndex=0;printIndex<HISTORY;printIndex++){
                            if(temphist[printIndex]!=0) printf("%d ",humhist[printIndex]);
                        }
                        printf("\nCo2: ");
                        for(printIndex=0;printIndex<HISTORY;printIndex++){
                            if(temphist[printIndex]!=0) printf("%d ",cohist[printIndex]);
                        }
                    }else{
                        printf("\nHistórico vazio\n");
                        return EMPTY_BUFFER;
                    }

                //ir buscar os arrays de historico

                return -10;  

            case CMD_RESET_HISTORY:
                
                for(int index=0;index<HISTORY;index++){
                    temphist[index] = '\0';
                    humhist[index] = '\0';
                    cohist[index] = '\0';
                    histIndex=0;
                }

                return -11; 
            



            default:
                return COMMAND_INVALID;
        }
            if(UARTRxBuff[i+1]!= 'p' && UARTRxBuff[i+1]!= 'a' && UARTRxBuff[i+1]!= 'l' && UARTRxBuff[i+1]!= 'r') return COMMAND_INVALID;

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

void getTxBuff(unsigned char * buf, int len)
{
    // Copy the length of the TX buffer to the variable pointed to by len
    len = txBufflen;
    
    // Check if the TX buffer contains any data
    if (txBufflen > 0) {
        // If the buffer is not empty, copy its contents to the buffer pointed to by buf
        memcpy(buf, UARTTxBuff, len);
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
       
        
        UARTTxBuff[lencheck]='0' + (checkot / 100);//tinha aqui len1 ja ns oq era
        txChar('0' + ((checkot / 10) % 10)); // Tens digit
        len++;
        txChar('0' + (checkot % 10)); // Units digit
        len++;
        txChar('!'); // End of frame        
        len++;       // Increment length
        // If the buffer is not full, fill the remaining spaces with null characters
        for (int k = len; k< UART_TX_SIZE; k++) {
        txChar('\0');
        }
        checkot = calcChecksum(UARTTxBuff, lencheck);
        if (checkin==checkot){
           return SUCCESS;
        }
        else {
            printf("2");
            return CHECKSUM_ERROR;
        }
    }
    else{
        printf("3");
        return CHECKSUM_ERROR;
    }

    }
    
    
    if(UARTRxBuff[i+1] == 'a'){
        txChar('a');
        len++;
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
       
        UARTTxBuff[lencheck]='0' + (checkot / 100);//tinha aqui len1 ja ns oq era
        txChar('0' + ((checkot / 10) % 10)); // Tens digit
        len++;
        txChar('0' + (checkot % 10)); // Units digit
        len++;
        txChar('!'); // End of frame        
        len++;       // Increment length
        // If the buffer is not full, fill the remaining spaces with null characters
        for (int k = len; k< UART_TX_SIZE; k++) {
        txChar('\0');
        }
        checkot = calcChecksum(UARTTxBuff, lencheck);
        if (checkin==checkot){
           return SUCCESS;
        }
        else 
        {
            printf("4");
            return CHECKSUM_ERROR;
            
        }
        
    }
    else{
        printf("5");
        return CHECKSUM_ERROR;
    }
        
    }
    if(UARTRxBuff[i+1] == 'l'){
        txChar('l');
        len++;
    }
    else{
        return COMMAND_INVALID;
        }


        
    }
    


int calcChecksum(unsigned char *buf, int nbytes) {
     // Compute the checksum as the sum of the numerical value of each byte in the buffer
    int checksum = 0;
    for (int i = 1; i < nbytes; i++) {
        if(buf[i]!='!' && buf[i]!='#'&& buf[i]!='\0')  checksum += buf[i];
        else checksum+=0;
    }

    // Compute the modulo 256 checksum
    checksum %= 256;

    // Return the checksum value
    return checksum;
}