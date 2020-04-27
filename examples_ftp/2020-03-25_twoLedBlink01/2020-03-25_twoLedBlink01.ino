// twoLedBlink01
/*
  Titilar dos LEDs con periodos diferentes:
  - ledA 2s
  - ledB 238ms
  Usando la función millis() para hacer un retardo no bloqueante
*/
bool ledStateA = LOW;
const int ledPinA = 2;
const int intervalA = 1000;
bool ledStateB = LOW;
const int ledPinB = 3;
const int intervalB = 119;

unsigned long prevMillisA = 0, prevMillisB = 0, currentMillis;

void setup() {
  pinMode(ledPinA, OUTPUT);
  digitalWrite(ledPinA, HIGH);
  pinMode(ledPinB, OUTPUT);
  digitalWrite(ledPinB, HIGH);
}

void loop() {
  currentMillis = millis();

  // Verificar si es tiempo de cambiar el estado del LED
  if ((currentMillis - prevMillisA) >= intervalA) {
    prevMillisA = currentMillis;
    ledStateA = !ledStateA;
    digitalWrite(ledPinA, ledStateA);
  }
  if ((currentMillis - prevMillisB) >= intervalB) {
    prevMillisB = currentMillis;
    ledStateB = !ledStateB;
    digitalWrite(ledPinB, ledStateB);
  }



}
