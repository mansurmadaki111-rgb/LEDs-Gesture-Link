/*
  NANO #1 - "GLOVE" (TRANSMITTER) - v2
  --------------------------------------
  Continuously sends a character over UART WHILE tilted:
    'L' = tilted left  (sent repeatedly every ~100ms while held)
    'R' = tilted right (sent repeatedly every ~100ms while held)
  Sends NOTHING while neutral - the receiver detects "no more signal"
  on its own using a timeout, and stops blinking.

  Wiring:
    GY-521 VCC -> 5V
    GY-521 GND -> GND
    GY-521 SCL -> A5
    GY-521 SDA -> A4

    Pin 11 (SoftwareSerial TX) -> Nano #2's Pin 10 (its SoftwareSerial RX)
    GND -> Nano #2 GND   (MUST be connected - shared ground)
*/

#include <Wire.h>
#include <SoftwareSerial.h>

SoftwareSerial LinkSerial(10, 11); // RX (unused), TX -> goes to Nano #2

const int MPU_ADDR = 0x68;
int16_t accX, accY, accZ;
float roll;
const float THRESHOLD = 20.0; // degrees
const int SEND_INTERVAL = 100; // ms between repeated sends while tilted

void setup() {
  Wire.begin();

  // Wake up the MPU6050
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);

  Serial.begin(9600);     // for debugging via USB
  LinkSerial.begin(9600); // for talking to Nano #2
}

void loop() {
  readAccel();
  roll = atan2(accY, accZ) * 180.0 / PI;

  if (roll > THRESHOLD) {
    LinkSerial.write('R');
    Serial.println("Sending: R");
  } else if (roll < -THRESHOLD) {
    LinkSerial.write('L');
    Serial.println("Sending: L");
  }
  // Neutral -> send nothing. Nano #2 will time out and stop on its own.

  delay(SEND_INTERVAL);
}

void readAccel() {
  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 6, true);

  accX = Wire.read() << 8 | Wire.read();
  accY = Wire.read() << 8 | Wire.read();
  accZ = Wire.read() << 8 | Wire.read();
}
