#include <xc.h> //ARQUIVO .READER COM AS CONFIGURAÇÕES DO uC

#pragma config FOSC = INTIO67 //OSCILADOR INTERNO
#pragma config PWRT = OFF
#pragma config WDTEN = OFF,LVP = OFF

#pragma config PBADEN = OFF

#define _XTAL_FREQ 16000000 //SETTANDO FREQUENCIA 1MHz
unsigned char chave;

void configurarUSART(){
    OSCCONbits.IRCF = 0b111;
    TXSTA = 0b00100000;
    RCSTA = 0b10010000;
    SPBRG = 208;
}

void __interrupt() ligar(){
    if (INTCONbits.INT0IF == 1){
        INTCONbits.INT0IF = 0;        
   }
}

void main(void) {
    TRISC = 0;

    //configurando o botao/int0
    TRISBbits.RB0 = 1;
    INTCONbits.INT0IE = 1;
    INTCONbits.INT0IF = 1;
    INTCON2bits.INTEDG0 = 0;
    RCONbits.IPEN = 0;
    INTCONbits.GIE = 1;
    
    //funcao que confira a comunicação serial
    configurarUSART();
    
    while(1){
        chave = PORTB;
        if (chave > 0){
            for(int i=0; i<10; i++){
                TXREG = 0x55; // Enviar '1' para ligar o LED
                
                // Espera concluir a transmissão do byte
                while (!(PIR1 & 0b00010000)){};
                __delay_ms(500);
            }
        }   
    }
    
    return;
}