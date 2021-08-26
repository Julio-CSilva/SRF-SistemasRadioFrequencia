#include <xc.h> //ARQUIVO .READER COM AS CONFIGURAÇÕES DO uC

#pragma config FOSC = INTIO67 //OSCILADOR INTERNO
#pragma config PWRT = OFF
#pragma config WDTEN = OFF,LVP = OFF

#pragma config PBADEN = OFF

#define _XTAL_FREQ 16000000 //SETTANDO FREQUENCIA 1MHz
unsigned char valor_recebido;
void configurarUSART(){
    OSCCONbits.IRCF = 0b111;
    TXSTA = 0b00100000;
    RCSTA = 0b10010000;
    SPBRG = 208;
}


void main(void) {
    
    TRISC = 1;
    
    TRISBbits.RB4 = 0;
    
    configurarUSART();
    
    while(1){
        RCSTA = 0b10010000; //prepara para receber o valor
        
        while((PIR1&32)==0){ //espera chegar um dado na serial
            valor_recebido = RCREG; //lê o valor recebido
            RCSTA = 0b10000000; //reseta a serial para uma nova recepção
            if(valor_recebido == 0x55){
                PORTBbits.RB4 = 1;
                __delay_ms(300);
                PORTBbits.RB4 = 0;
            }
          
        }
    
    }
    
    
    
    
    return;
}