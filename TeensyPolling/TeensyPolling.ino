/*
    Reads the frequency of an incoming PWM signal with polling.
    The frequency is converted into a capacitance value.
    Polling is ~350 kHz, max frequency is ~175 kHz. Accuracy to
    within +/- 0.5*collectionsPerSecond.

    This would be used along with the 555 timer circuit that converts
    variable capacitance into variable frequency.

    This program was originally designed for use with a Teensy 3.2,
    overclocked to 120 MHz, but it may work with other boards with some
      modifications.
*/

const bool verboseOut = false; // More debug info (recommended: false)
const bool usePlotter = true; // Use Serial Plotter (recommended: true)

const int ledPin = 13;
const int inPin = 2;
const int collectionsPerSecond = 50;
const int collectionTime = (1000 / collectionsPerSecond) * 1000;
const int valueR1 = 554; // ohms
const int valueR2 = 559; // ohms

bool lastState = true;
bool currentState = true;

int numCycles = 0;
int numMeasurements = 0;

float startMicros = 0;
float stopMicros = 0;
float freq = 0;
float cap = 0;

void setup() {

  pinMode(ledPin, OUTPUT);
  pinMode(inPin, INPUT);

  Serial.begin(2000000);
  Serial.println("begin");
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
  cap = 1 / (freq * 0.693 * (valueR1 + (2 * valueR2)));
  if (verboseOut) {
    Serial.print("startMicros: ");
    Serial.print(startMicros);
    Serial.print(" stopMicros: ");
    Serial.print(stopMicros);
    Serial.print(" numMeasurements: ");
    Serial.print(numMeasurements);
    Serial.print(" numCycles: ");
    Serial.print(numCycles);
    Serial.print(" freq: ");
    Serial.print(freq);
  }
  if (usePlotter) {
    Serial.println(cap * 1000000000);
  }
  else {
    Serial.print(" cap: ");
    Serial.print(cap * 1000000000);
    Serial.println("nF");
  }
  numCycles = 0;
  numMeasurements = 0;
}
