void setup() {
  DDRH = DDRH|B00000010; //configura RH1 como saída(pino Tx2)
  UCSR2A = 0x00; //limpa as flags TXC e RXC, configura para velocidade normal
  UCSR2C = 0b00000110; //formato UART assíncrono - 8N1
  UCSR2B = (1<<TXEN0); //habilita TX
  UBRR2 = 103; //baud rate 9600bps
}

void loop() {
  while (!(UCSR2A & (1<<UDRE2))){};
  UDR2 = 0x64; // 'a'
  delay(1000);
  
  while (!(UCSR2A & (1<<UDRE2))){};
  UDR2 = 0x63; // 'b'
  delay(1000);
  
  while (!(UCSR2A & (1<<UDRE2))){};
  UDR2 = 0x62; // 'c'
  delay(1000);

  while (!(UCSR2A & (1<<UDRE2))){};
  UDR2 = 0x61; // 'd'
  delay(1000);
}