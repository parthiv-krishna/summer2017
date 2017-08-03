/*
    Switch to the desired multiplexer pin. Input over Serial.

    This would be used to measure 8 capacitance values with only one
    sensor or measurement circuit.

    This program was originally designed for use with the Teensy 3.2,
    but it may work with other boards with some modifications.
*/

const int muxPins[3] = {20, 21, 22}; // Pins that S0, S1, and S2 are connected to respectively. Shouldn't have to be changed.

byte serialIn = 0; // input from Serial monitor
int selectedPin = 0; // parsed integer 

void setup() {
  for (int k = 0; k < 3; k++) {  // intialize all selection pins
    pinMode(muxPins[k], OUTPUT);
    digitalWrite(muxPins[k], LOW);
  }
  Serial.begin(2000000);
  Serial.println("Enter a number between 0 and 7 to switch to that multiplexer pin");
}

void loop() {
  if (Serial.available() > 0) { // if something was inputted to the Serial monitor
    serialIn = Serial.read();
    selectedPin = serialIn - 48; // convert ASCII byte to integer
    if (selectedPin < 0 || selectedPin > 7) {
      Serial.println("input outside range"); 
    }
    else {
      Serial.print(serialIn);  // feedback
      selectMuxPin(selectedPin);
      Serial.print(" selected pin ");
      Serial.println(selectedPin);

    }
  }
}

void selectMuxPin(byte pin) {  // set appropriate selection pins based on desired multiplexer pin
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
