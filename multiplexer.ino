/* 
 *  Cycle through all multiplexer (74HC4051) outputs. Since the 74HC4051
 *  has 8 outputs, it cycles from 0-7. 
 *  
 *  This would be used to measure 8 capacitance values with only one
 *  sensor or measurement circuit.
 *  
 *  This program was originally designed for use with the Teensy 3.2,
 *  but it may work with other boards with some modifications.
 */

const int muxPins[3] = {20, 21, 22};
const int muxZ = 19;
const int delayTime = 1000;

void setup() {
  for (int k = 0; k < 3; k++) {
    pinMode(muxPins[k], OUTPUT);
    digitalWrite(muxPins[k], LOW);
  }
  pinMode(muxZ, OUTPUT);
  digitalWrite(muxZ, HIGH);
}

void loop() {
  for (int i = 0; i < 7; i++) {
    selectMuxPin(i);
    delay(delayTime);
  }
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
