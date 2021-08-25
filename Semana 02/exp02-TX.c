#include <xc.h> //ARQUIVO .READER COM AS CONFIGURAÇÕES DO uC

#pragma config FOSC = INTIO67 //OSCILADOR INTERNO
#pragma config PWRT = OFF
#pragma config WDTEN = OFF
#pragma config PBADEN = OFF

#define _XTAL_FREQ 16000000 //SETTANDO FREQUENCIA 1MHz
unsigned char valor;

void configurarUSART(){
    OSCCONbits.IRCF = 0b111;
    TXSTA = 0b00100000;
    RCSTA = 0b10010000;
    SPBRG = 26;
}

void __interrupt() ligar(){
    if (INTCONbits.INT0IF == 1){
        INTCONbits.INT0IF = 0;
        
        
    }
}

void main(void) {
    TRISCbits.RC7 = 1;
    TRISD = 0;
    TRISBbits.RB0 = 1;
    
    INTCONbits.INT0IE = 1;
    INTCONbits.INT0IF = 1;
    INTCON2bits.INTEDG0 = 0;
    
    RCONbits.IPEN = 0;
    INTCONbits.GIE = 1;
    
    configurarUSART();
    
    PORTCbits.RC6 = 0;
    
    while(1){
        if (PORTBbits.RB0 == 0){
            TXREG = 0x49; // Enviar '1' para ligar o LED
        } else{
            TXREG = 0x48; // Enviar '0' para ligar o LED
        }
        // Espera concluir a transmissão do byte
        while (!(PIR1 & 0b00010000)){};
    }
    
    
    
    return;
}