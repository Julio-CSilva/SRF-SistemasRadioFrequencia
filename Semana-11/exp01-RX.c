#include <xc.h> //ARQUIVO .READER COM AS CONFIGURAÇÕES DO uC

#pragma config FOSC = INTIO67 //OSCILADOR INTERNO
#pragma config PWRT = OFF
#pragma config WDTEN = OFF

#define _XTAL_FREQ 16000000 //SETTANDO FREQUENCIA 1MHz
unsigned char valor;

void configurarUSART(){
    OSCCONbits.IRCF = 0b111;
    TXSTA = 0b00100000;
    RCSTA = 0b10010000;
    SPBRG = 26;
}


void main(void) {
    TRISCbits.RC7 = 1;
    TRISD = 0;
    
    configurarUSART();
    
    while(1){
        while(PIR1bits.RCIF == 0){
        }
        valor = RCREG;
        
        if(valor == 0x61){
            PORTD = 0b00000001;
        }
        if(valor == 0x62){
            PORTD = 0b00000010;
        }
        if(valor == 0x63){
            PORTD = 0b00000100;
        }
        if(valor == 0x64){
            PORTD = 0b00001000;
        }
    }
    
    return;
}