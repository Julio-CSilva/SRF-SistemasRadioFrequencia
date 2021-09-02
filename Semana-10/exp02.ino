void setup() {
  DDRE = DDRE | B00000010; // Configura RE1 como saída (pino TX0)

  UCSR0A = 0x00; // Limpa flags TXC RXC; Configura para velocidade normal.

  UCSR0C = 0b00000110; // Formato: UART assíncrono - 8N1.
  
  UCSR0B = (1<<TXEN0); // Habilita TX.

  UBRR0 = 103; // Baud rate 9600 bps (16 MHz; U2X (veloc normal)).
}

void loop() {
  while (!(UCSR0A & (1<<UDRE0))){}; // Aguardando o buffer de transmissão vazio.
  UDR0 = 0x61; // Enviando o caractere ‘a’ (tabela ASCII).
  
  delay(1000);

}