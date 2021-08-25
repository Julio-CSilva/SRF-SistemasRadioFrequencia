void setup() {
  Serial2.begin(9600);
  pinMode(11, OUTPUT);
}
void loop() {
  byte x = Serial2.read();
  if (x == 0x49){
    digitalWrite(11,HIGH);
  } else{
    digitalWrite(11,LOW);
  }
}