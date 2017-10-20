// Change these.
#define RESISTOR_VALUE  46750.0F    // RC resistance
#define OUTPUT_MODE 0  // options: CAP_TO_MONITOR, CAP_TO_PLOTTER, VOLT_TO_PLOTTER
#define CHARGE_TIME_CONSTANTS 1

// Pin definitions.
#define ANALOG_IN       1           // analog input for measuring the voltage 
#define CHARGE_PIN      10          // digital pin to charge capacitor
#define DISCHARGE_PIN   9           // digital pin to discharge capacitor
#define LED_PIN         13          // charge status LED

// Don't change anything below this line.
/******************************************************************************/

#define CAP_TO_MONITOR 0
#define CAP_TO_PLOTTER 1
#define VOLT_TO_PLOTTER 2

const int maxCharge = 1023 * (1 - pow(0.36788, CHARGE_TIME_CONSTANTS));

unsigned long startTime;
unsigned long elapsedTime;
float timeConstant;
float microFarads;
float nanoFarads;

// capacitance measurement smoothing
const int numReadings = 5;
int readIndex = 0;
int total = 0;
int runningAverage = 0;
float latestCaps[numReadings];

void setup() {
  pinMode(CHARGE_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  digitalWrite(CHARGE_PIN, LOW);

  for (int i = 0; i < numReadings; i++) {
    latestCaps[i] = 0;
  }

  Serial.begin(2000000);
  Serial.println("begin");
}

void loop() {
  if (OUTPUT_MODE == CAP_TO_MONITOR) {
    capacitanceToMonitor();
  }
  else if (OUTPUT_MODE == CAP_TO_PLOTTER) {
    capacitanceToPlotter();
  }
  else if (OUTPUT_MODE == VOLT_TO_PLOTTER) {
    voltageToPlotter();
  }
  else {
    Serial.println("No mode selected");
    while (1) {
    }
  }
}

void capacitanceToMonitor() {
  Serial.println("----------------------------------------------------------------");

  // charge
  digitalWrite(CHARGE_PIN, HIGH);  // set CHARGE_PIN HIGH and capacitor charging
  digitalWrite(LED_PIN, HIGH);
  Serial.println("charging");
  startTime = micros();
  while (analogRead(ANALOG_IN) < maxCharge) {     // charge until 647 (63.2% of 1023)
  }
  elapsedTime = micros() - startTime;
  timeConstant = elapsedTime / CHARGE_TIME_CONSTANTS;

  // convert microseconds to seconds ( 10^-6 ) and Farads to microFarads ( 10^6 ), they cancel
  microFarads = ((float)timeConstant / RESISTOR_VALUE);
  Serial.print(CHARGE_TIME_CONSTANTS);
  Serial.print(" time constant(s)    ");
  Serial.print(elapsedTime);
  Serial.print(" uS    ");
  nanoFarads = microFarads * 1000.0;
  Serial.print((long)nanoFarads);
  Serial.println(" nanoFarads");
  runningAverage = normalizeReadings(latestCaps, nanoFarads);
  Serial.print("Running Average (last ");
  Serial.print(numReadings);
  Serial.print(" readings): ");
  if (numReadings < 10) {
    Serial.print(" ");
  }
  Serial.print(runningAverage);
  Serial.println(" nanoFarads");
  
  // discharge
  Serial.println("discharging");
  digitalWrite(LED_PIN, LOW);
  digitalWrite(CHARGE_PIN, LOW);
  pinMode(DISCHARGE_PIN, OUTPUT);
  digitalWrite(DISCHARGE_PIN, LOW);
  while (analogRead(ANALOG_IN) > 10) {       // wait until capacitor is completely discharged
  }
  pinMode(DISCHARGE_PIN, INPUT);
}

void capacitanceToPlotter() {
  // charge
  digitalWrite(CHARGE_PIN, HIGH);  // set CHARGE_PIN HIGH and capacitor charging
  digitalWrite(LED_PIN, HIGH);
  startTime = micros();
  while (analogRead(ANALOG_IN) < maxCharge) {     // charge until 647 (63.2% of 1023)
  }
  elapsedTime = micros() - startTime;
  timeConstant = elapsedTime / CHARGE_TIME_CONSTANTS;

  // convert microseconds to seconds ( 10^-6 ) and Farads to nanoFarads ( 10^6 ), net 10^3 = 1000
  nanoFarads = ((float)timeConstant / RESISTOR_VALUE) * 100;
  runningAverage = normalizeReadings(latestCaps, nanoFarads);
  Serial.println((long)runningAverage);

  // discharge
  digitalWrite(CHARGE_PIN, LOW);
  digitalWrite(LED_PIN, LOW);
  pinMode(DISCHARGE_PIN, OUTPUT);
  digitalWrite(DISCHARGE_PIN, LOW);
  while (analogRead(ANALOG_IN) > 2) {       // wait until capacitor is completely discharged
  }
  pinMode(DISCHARGE_PIN, INPUT);
}

void voltageToPlotter() {
  // charge
  digitalWrite(CHARGE_PIN, HIGH);  // set CHARGE_PIN HIGH and capacitor charging
  digitalWrite(LED_PIN, HIGH);
  startTime = millis();
  while (analogRead(ANALOG_IN) < maxCharge) {     // charge until 647 (63.2% of 1023)
    Serial.println(analogRead(ANALOG_IN));
    delay(5);                               // to avoid overloading the serial port
  }
  elapsedTime = millis() - startTime;
  timeConstant = elapsedTime / CHARGE_TIME_CONSTANTS;

  // convert milliseconds to seconds ( 10^-3 ) and Farads to microFarads ( 10^6 ),  net 10^3 (1000)
  microFarads = ((float)timeConstant / RESISTOR_VALUE) * 1000;
  nanoFarads = microFarads * 1000.0;

  // discharge
  digitalWrite(CHARGE_PIN, LOW);
  digitalWrite(LED_PIN, LOW);
  pinMode(DISCHARGE_PIN, OUTPUT);
  digitalWrite(DISCHARGE_PIN, LOW);
  while (analogRead(ANALOG_IN) > 2) {       // wait until capacitor is completely discharged
    Serial.println(analogRead(ANALOG_IN));
    delay(5); // to avoid overloading the serial port
  }
  pinMode(DISCHARGE_PIN, INPUT);
}

float normalizeReadings(float readings[], float lastReading) {
  total = total - readings[readIndex];  
  readings[readIndex] = lastReading;
  total = total + readings[readIndex];
  readIndex = readIndex + 1;
  // if at the end of the array, return to beginning
  if (readIndex >= numReadings) {
    readIndex = 0;
  }
  float average = total / numReadings;
  return average;
}
