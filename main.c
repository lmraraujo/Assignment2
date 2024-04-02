#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cmdproc.h"

static unsigned char userBuff[20];

int main(void){

    while(1){
        resetRxBuff();

        printf("\nInsere um comando válido:\n");
        int k;
        fgets(userBuff,20,stdin);
        for (k = 0; userBuff[k]!='\0'; k++) {
            rxChar(userBuff[k]);
        }
    
        cmdProc();
    }
    

}