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

const int numCaps = 1;

const int ledPin = 13; // Onboard LED on Teensy
const int inPin = 0; // Input from 555 Timer
const int muxPins[3] = {20, 21, 22};

const int collectionsPerSecond = 50; // Increase for more readings but less accuracy.
const int collectionTime = (1000 / collectionsPerSecond) * 1000; // Don't change!

// Hardware information
const float valueR1 = 554; // ohms
const float valueR2 = 559; // ohms

/* ------------------------------------------------------------------------------------ */

bool lastState = true;
bool currentState = true;

int numCycles = 0;
int numMeasurements = 0;

float startMicros = 0;
float stopMicros = 0;
float freq = 0;

float caps[8] = {0, 0, 0, 0, 0, 0, 0, 0};

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(inPin, INPUT);

  Serial.begin(2000000);
  Serial.println("begin");
  digitalWrite(ledPin, HIGH);
}

void loop() {
  if (numCaps > 1) {
    for (int i = 0; i < numCaps; i++) {
      selectMuxPin(i);
      caps[i] = getCapNF();
    }
    for (int i = 0; i < (numCaps - 1); i++) {
      Serial.print(caps[i]);
      Serial.print(",");
    }
  }
  else {
    selectMuxPin(0);
    caps[0] = getCapNF();
  }
  Serial.println(caps[(numCaps - 1)]);
}

void selectMuxPin(byte pin) {
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

float getCapNF() {
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
  float cap = 1 / (freq * 0.693 * (valueR1 + (2 * valueR2)));
  numCycles = 0;
  numMeasurements = 0;
  return (cap * 1000000000);
}
