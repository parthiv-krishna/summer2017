/*
 *    Reads the capacitance of eight multiplexed capacitors.
 *
 *    Used with the 555 timer circuit and a 74HC4051 8-channel multiplexer.
 *
 *    This program was originally designed for use with a Teensy 3.2
 *    but it may work with other boards with some modifications.
 */

const bool verboseOut = false;

const int ledPin = 13;
const int inPin = 2;
const int collectionsPerSecond = 1; // overall. per pin is 1/8 of this.
const int collectionTime = (1000 / collectionsPerSecond) * 1000;
const int valueR1 = 554; // ohms
const int valueR2 = 559; // ohms
const int muxPins[3] = {20, 21, 22};

bool lastState = true;
bool currentState = true;

int numCycles = 0;
int numMeasurements = 0;
int currentMuxPin = 0;

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

  for (int i = 0; i < 3; i++) {
    pinMode(muxPins[i], OUTPUT);
    digitalWrite(muxPins[i], LOW);
  }
}

void loop() {
  setMux(currentMuxPin);
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
  Serial.print("Pin: ");
  Serial.print(currentMuxPin);
  if (verboseOut) {
    Serial.print(" startMicros: ");
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
  Serial.print(" cap: ");
  Serial.print(cap * 1000000000);
  Serial.println("nF");
  numCycles = 0;
  numMeasurements = 0;
  currentMuxPin++;
  if (currentMuxPin > 7) {
    currentMuxPin = 0;
  }
}

void setMux(byte pin) {
  if (pin > 7) {
    return;
  }
  for (int j = 0; j < 3; j++) {
    if (pin & (1 << j)) {
      digitalWrite(muxPins[j], HIGH);
    }
    else {
      digitalWrite(muxPins[j], LOW);
    }
  }
}
