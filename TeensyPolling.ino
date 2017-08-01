/*  
 *  Reads the frequency of an incoming PWM signal with polling.
 *  Polling is ~350 kHz, max frequency is ~175 kHz. Accuracy to 
 *  within +/- 0.5*collectionsPerSecond.
 *  
 *  This would be used along with the 555 timer circuit that converts
 *  variable capacitance into variable frequency.
 *  
 *  This program was originally designed for use with a Teensy 3.2
 *  but it may work with other boards with some modifications.
 */

const int ledPin = 13;
const int inPin = 2;
const int collectionsPerSecond = 1000;
const int collectionTime = (1000 / collectionsPerSecond) * 1000;

bool lastState = true;
bool currentState = true;

int numCycles = 0;
int numMeasurements = 0;

float startMicros = 0;
float stopMicros = 0;
float freq = 0;

void setup() {

pinMode(ledPin, OUTPUT);
pinMode(inPin, INPUT);

  Serial.begin(2000000);
Serial.print("begin");
digitalWrite(ledPin, HIGH);
}

void loop() {
  startMicros = micros();
  stopMicros = startMicros + collectionTime;
  while (micros() < stopMicros) {
    currentState = digitalRead(inPin);
    if (!lastState) {
      if (currentState) {
        numCycles ++;
      }
    }
    lastState = currentState;
    numMeasurements++;
  }
  freq = numCycles * collectionsPerSecond;
  Serial.print("startMicros: ");
  Serial.print(startMicros);
  Serial.print(" stopMicros: ");
  Serial.print(stopMicros);
  Serial.print(" numMeasurements");
  Serial.print(numMeasurements);
  Serial.print(" numCycles: "); 
  Serial.print(numCycles);
  Serial.print(" freq: ");
  Serial.println(freq);
  numCycles = 0;
  numMeasurements = 0;
}
