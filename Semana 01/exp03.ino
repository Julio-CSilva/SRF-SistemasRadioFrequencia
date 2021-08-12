void setup() {
  DDRE = DDRE | B00000010; //RE1(TX0) como saída
  UCSR0A = 0x00; //Limpa as flags
  UCSR0C = 0b00000110; // Escolhe o formato UART que é assíncrono - 8N1.
  UCSR0B = (1<<TXEN0); // Habilitando TX.
  UBRR0 = 103; // Baud rate 9600 bps.
}

void loop() {
  while (!(UCSR0A & (1<<UDRE0))){}; // Aguardando o buffer de transmissão vazio.
  
  UDR0 = 0x4f; // ATMEGA2560 Enviando o caractere ‘O’ pela tabela ASCII
  UDR0 = 0x69; // ATMEGA2560 Enviando o caractere ‘i’ pela tabela ASCII
  delay(100);
  UDR0 = 0x21; // ATMEGA2560 Enviando o caractere ‘!’ pela tabela ASCII
  UDR0 = 0x20; // ATMEGA2560 Enviando o caractere ‘ ’ pela tabela ASCII
  delay(1900);

}