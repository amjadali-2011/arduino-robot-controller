/*
  Arduino Robot Controller — Arduino 1
  Hardware notes based on the previously discussed wiring:
    L298N motor inputs: D2, D3, D4, D5
    Lock/output control: D6
    HC-05 Bluetooth: module TX -> Arduino D7; Arduino D8 -> module RX
      Use a voltage divider on Arduino D8 -> HC-05 RX.
    Pulse sensor signal: A0
    DS18B20 data: A2 (requires OneWire + DallasTemperature libraries
      and a suitable pull-up resistor for real temperature readings)

  Bluetooth commands:
    F = forward, B = backward, L = left, R = right, S = stop
    O = lock/output ON, X = lock/output OFF
    T = print sensor telemetry

  Important:
    - Test with wheels lifted off the ground first.
    - This sketch uses a clearly marked demo temperature value because
      the DS18B20 was not being detected in the previous test.
    - BPM is a rough threshold-crossing estimate, not a medical reading.
*/

#include <SoftwareSerial.h>

const uint8_t MOTOR_IN1 = 2;
const uint8_t MOTOR_IN2 = 3;
const uint8_t MOTOR_IN3 = 4;
const uint8_t MOTOR_IN4 = 5;
const uint8_t LOCK_PIN  = 6;

const uint8_t HC05_RX_PIN = 7; // Arduino receives from HC-05 TX
const uint8_t HC05_TX_PIN = 8; // Arduino transmits to HC-05 RX through divider
const uint8_t PULSE_PIN   = A0;

SoftwareSerial bluetooth(HC05_RX_PIN, HC05_TX_PIN);

const bool USE_DEMO_TEMPERATURE = true;
const float DEMO_TEMPERATURE_C = 37.2;

int pulseRaw = 0;
int bpm = 0;
bool pulseWasHigh = false;
unsigned long lastBeatMs = 0;
unsigned long lastTelemetryMs = 0;

void stopMotors() {
  digitalWrite(MOTOR_IN1, LOW);
  digitalWrite(MOTOR_IN2, LOW);
  digitalWrite(MOTOR_IN3, LOW);
  digitalWrite(MOTOR_IN4, LOW);
}

void moveForward() {
  digitalWrite(MOTOR_IN1, HIGH);
  digitalWrite(MOTOR_IN2, LOW);
  digitalWrite(MOTOR_IN3, HIGH);
  digitalWrite(MOTOR_IN4, LOW);
}

void moveBackward() {
  digitalWrite(MOTOR_IN1, LOW);
  digitalWrite(MOTOR_IN2, HIGH);
  digitalWrite(MOTOR_IN3, LOW);
  digitalWrite(MOTOR_IN4, HIGH);
}

void turnLeft() {
  digitalWrite(MOTOR_IN1, LOW);
  digitalWrite(MOTOR_IN2, HIGH);
  digitalWrite(MOTOR_IN3, HIGH);
  digitalWrite(MOTOR_IN4, LOW);
}

void turnRight() {
  digitalWrite(MOTOR_IN1, HIGH);
  digitalWrite(MOTOR_IN2, LOW);
  digitalWrite(MOTOR_IN3, LOW);
  digitalWrite(MOTOR_IN4, HIGH);
}

void printTelemetry(Stream &out) {
  out.print(F("PULSE_RAW="));
  out.print(pulseRaw);
  out.print(F(",BPM_EST="));
  out.print(bpm);
  out.print(F(",TEMP_C="));
  if (USE_DEMO_TEMPERATURE) {
    out.print(DEMO_TEMPERATURE_C, 1);
    out.print(F(" (DEMO)"));
  } else {
    out.print(F("DS18B20_NOT_CONFIGURED"));
  }
  out.println();
}

void updatePulseEstimate() {
  pulseRaw = analogRead(PULSE_PIN);

  // Simple rising-threshold detector. Adjust threshold for your sensor.
  const int threshold = 600;
  bool pulseIsHigh = pulseRaw > threshold;
  unsigned long now = millis();

  if (pulseIsHigh && !pulseWasHigh && (now - lastBeatMs) > 300) {
    if (lastBeatMs != 0) {
      unsigned long interval = now - lastBeatMs;
      int calculatedBpm = (int)(60000UL / interval);
      if (calculatedBpm >= 35 && calculatedBpm <= 220) {
        bpm = calculatedBpm;
      }
    }
    lastBeatMs = now;
  }
  pulseWasHigh = pulseIsHigh;
}

void handleCommand(char command) {
  switch (command) {
    case 'F': case 'f':
      moveForward();
      bluetooth.println(F("MOVING_FORWARD"));
      Serial.println(F("MOVING_FORWARD"));
      break;
    case 'B': case 'b':
      moveBackward();
      bluetooth.println(F("MOVING_BACKWARD"));
      Serial.println(F("MOVING_BACKWARD"));
      break;
    case 'L': case 'l':
      turnLeft();
      bluetooth.println(F("TURNING_LEFT"));
      Serial.println(F("TURNING_LEFT"));
      break;
    case 'R': case 'r':
      turnRight();
      bluetooth.println(F("TURNING_RIGHT"));
      Serial.println(F("TURNING_RIGHT"));
      break;
    case 'S': case 's':
      stopMotors();
      bluetooth.println(F("STOPPED"));
      Serial.println(F("STOPPED"));
      break;
    case 'O': case 'o':
      digitalWrite(LOCK_PIN, HIGH);
      bluetooth.println(F("OUTPUT_ON"));
      Serial.println(F("OUTPUT_ON"));
      break;
    case 'X': case 'x':
      digitalWrite(LOCK_PIN, LOW);
      bluetooth.println(F("OUTPUT_OFF"));
      Serial.println(F("OUTPUT_OFF"));
      break;
    case 'T': case 't':
      printTelemetry(bluetooth);
      printTelemetry(Serial);
      break;
    case '\n': case '\r': case ' ':
      break;
    default:
      bluetooth.print(F("UNKNOWN_COMMAND: "));
      bluetooth.println(command);
      Serial.print(F("UNKNOWN_COMMAND: "));
      Serial.println(command);
      break;
  }
}

void setup() {
  pinMode(MOTOR_IN1, OUTPUT);
  pinMode(MOTOR_IN2, OUTPUT);
  pinMode(MOTOR_IN3, OUTPUT);
  pinMode(MOTOR_IN4, OUTPUT);
  pinMode(LOCK_PIN, OUTPUT);
  pinMode(PULSE_PIN, INPUT);

  stopMotors();
  digitalWrite(LOCK_PIN, LOW);

  Serial.begin(9600);
  bluetooth.begin(9600);

  Serial.println(F("Arduino 1 Robot Controller Ready"));
  bluetooth.println(F("Arduino 1 Robot Controller Ready"));
  bluetooth.println(F("Commands: F B L R S O X T"));
}

void loop() {
  updatePulseEstimate();

  while (bluetooth.available() > 0) {
    handleCommand((char)bluetooth.read());
  }

  while (Serial.available() > 0) {
    handleCommand((char)Serial.read());
  }

  if (millis() - lastTelemetryMs >= 1000) {
    lastTelemetryMs = millis();
    // Stream periodic pulse readings to both terminals.
    printTelemetry(Serial);
    printTelemetry(bluetooth);
  }
}
