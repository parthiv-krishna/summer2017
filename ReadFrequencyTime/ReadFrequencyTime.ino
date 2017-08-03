/*  
 *  Reads the frequency of an incoming PWM signal.
 *  
 *  This would be used along with the 555 timer circuit that converts
 *  variable capacitance into variable frequency.
 *  
 *  This program was originally designed for use with an Arduino Uno,
 *  but it may work with other boards with some modifications. Unfortunately, 
 *  the Arduino is not fast enough to do very many readings per second, so this 
 *  program only reads it once and requires a reset for multiple readings.
 */

const int intPin = 2;
const int ledPin = 13;
const int collectionsPerSecond = 50; 
const int collectionTime = (1000/collectionsPerSecond) * 1000;

volatile int numCycles;
float startMicros = 0;
float stopMicros = 0;
float freq = 0;


void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  Serial.begin(9600);
  Serial.println("begin");
  startMicros = micros();
  stopMicros = startMicros + collectionTime;
  attachInterrupt(digitalPinToInterrupt(intPin), increment, RISING);
  while (micros() < stopMicros) {
  }
  detachInterrupt(digitalPinToInterrupt(intPin));
  freq = numCycles * collectionsPerSecond;
  Serial.print("startMicros: ");
  Serial.print(startMicros);
  Serial.print(" stopMicros: ");
  Serial.print(stopMicros);
  Serial.print(" numCycles: "); 
  Serial.print(numCycles);
  Serial.print(" freq: ");
  Serial.println(freq);
}

void loop() {
}

void increment() {
  numCycles ++;
}
