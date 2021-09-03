#include <SPI.h>

int v_recebido = 0;

int LED1 = 13;
int LED2 = 12;
int LED3 = 11;
int LED4 = 10;

void setup(){
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  
  SPCR |= bit(SPE); // Configura modo escravo
  pinMode(MISO, OUTPUT); // Pino MISO para saída
  pinMode(MOSI, INPUT); // Pino MOSI como entrada
  SPI.attachInterrupt(); // Configura interrupção do SPI
}

ISR (SPI_STC_vect){ // Função de interrupção
  v_recebido = SPDR;
  Serial.println(v_recebido);
}

void loop (void){
  // Lógica para ligar os LEDs de acordo com o valor SPI
  if (v_recebido == 1){
    LED1 = HIGH;
    LED2 = LOW;
    LED3 = LOW;
    LED4 = LOW;
  } else if (v_recebido == 2) {
    LED1 = LOW;
    LED2 = HIGH;
    LED3 = LOW;
    LED4 = LOW;
  } else if (v_recebido == 4) {
    LED1 = LOW;
    LED2 = LOW;
    LED3 = HIGH;
    LED4 = LOW;
  } else if (v_recebido == 8) {
    LED1 = LOW;
    LED2 = LOW;
    LED3 = LOW;
    LED4 = HIGH;
  }
}