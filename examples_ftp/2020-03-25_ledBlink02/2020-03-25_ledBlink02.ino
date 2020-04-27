// ledBlink02
/*
  Titilar un LED con periodo de 2s:
  - 1s encendido
  - 1s apagado
  Usando la función millis() para hacer un retardo no bloqueante
*/
bool ledStateA = LOW;
const int ledPinA = 2;
const int intervalA = 1000;
unsigned long prevMillisA = 0, currentMillis;

bool ledStateb = LOW;
const int ledPinB = 2;
const int intervalB = 1000;


void setup() {
  pinMode(ledPinA, OUTPUT);
  digitalWrite(ledPinA, HIGH);
}

void loop() {
  currentMillis = millis();

  // Verificar si es tiempo de cambiar el estado del LED
  if ((currentMillis - prevMillisA) >= intervalA) {
    prevMillisA = currentMillis;
    ledStateA = !ledStateA;
    digitalWrite(ledPinA, ledStateA);
  }



}
