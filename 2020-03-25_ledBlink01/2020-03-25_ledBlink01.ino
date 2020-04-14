// ledBlink01
/*
Titilar un LED con periodo de 2s:
- 1s encendido
- 1s apagado
*/
bool ledState = LOW;
const int ledPin = 2;
const int interval = 1000;


void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin, OUTPUT);
  //digitalWrite(ledPin, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  ledState = !ledState;
  digitalWrite(ledPin, ledState);
  delay(interval);


}
