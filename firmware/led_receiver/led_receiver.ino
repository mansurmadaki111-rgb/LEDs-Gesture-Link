/*
  NANO #2 - "LED UNIT" (RECEIVER) - v2
  ---------------------------------------
  Keeps cycling the LED sequence (1->2->3->4->1->2...) continuously
  WHILE it keeps receiving 'L' or 'R'.
  Stops and turns off automatically if no command arrives for a short
  timeout (meaning the glove has returned to neutral / stopped sending).

  Uses millis() timing instead of delay() so it can keep listening for
  new incoming Bluetooth/UART data while a sequence is running.

  Wiring:
    Pin 10 (SoftwareSerial RX) <- Nano #1's Pin 11 (its SoftwareSerial TX)
    GND -> Nano #1 GND   (MUST be connected - shared ground)

    LEDs: pin -> 220ohm resistor -> LED anode -> LED cathode -> GND
    Left set:  pins 2, 3, 4, 5
    Right set: pins 6, 7, 8, 9
*/

#include <SoftwareSerial.h>

SoftwareSerial LinkSerial(10, 11); // RX <- comes from Nano #1, TX (unused)

int leftLEDs[4]  = {2, 3, 4, 5};
int rightLEDs[4] = {6, 7, 8, 9};

const unsigned long STEP_DELAY = 150;  // ms per LED in the sequence
const unsigned long TIMEOUT   = 300;   // ms of silence before stopping
                                        // (must be > transmitter SEND_INTERVAL)

char currentCommand = 'N';   // 'N' = neutral, 'L' = left, 'R' = right
unsigned long lastReceiveTime = 0;
unsigned long lastStepTime = 0;
int stepIndex = 0;

void setup() {
  Serial.begin(9600);      // for debugging via USB
  LinkSerial.begin(9600);  // for listening to Nano #1

  for (int i = 0; i < 4; i++) {
    pinMode(leftLEDs[i], OUTPUT);
    pinMode(rightLEDs[i], OUTPUT);
  }
}

void loop() {
  // 1. Check for incoming command (non-blocking)
  if (LinkSerial.available()) {
    char c = LinkSerial.read();
    if (c == 'L' || c == 'R') {
      currentCommand = c;
      lastReceiveTime = millis();
    }
  }

  // 2. If no command received recently, go neutral and turn off
  if (millis() - lastReceiveTime > TIMEOUT) {
    if (currentCommand != 'N') {
      Serial.println("Signal lost - stopping");
    }
    currentCommand = 'N';
    turnOffAll();
    stepIndex = 0;
  }

  // 3. If a command is active, keep advancing the LED sequence
  if (currentCommand != 'N' && millis() - lastStepTime >= STEP_DELAY) {
    lastStepTime = millis();
    turnOffAll();

    int* activeLEDs = (currentCommand == 'L') ? leftLEDs : rightLEDs;
    digitalWrite(activeLEDs[stepIndex], HIGH);

    Serial.print("Blink step: ");
    Serial.print(currentCommand);
    Serial.println(stepIndex + 1);

    stepIndex = (stepIndex + 1) % 4;
  }
}

void turnOffAll() {
  for (int i = 0; i < 4; i++) {
    digitalWrite(leftLEDs[i], LOW);
    digitalWrite(rightLEDs[i], LOW);
  }
}
