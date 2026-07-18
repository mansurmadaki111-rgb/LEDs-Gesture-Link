# LEDs-Gesture-Link

**A wearable hand-gesture control system built with Arduino Nano and an MPU6050 (GY-521) IMU.**

LEDs-Gesture-Link reads hand tilt from a glove-mounted motion sensor and wirelessly triggers actions on a receiving unit — currently an LED indicator array, with a gesture-controlled obstacle-avoidance robot car planned as the end goal.

---

## 📌 Project Status

This repo is being built in phases. Current phase: **Phase 2 — Wired UART link between two Nanos.**

| Phase | Description | Status |
|-------|-------------|--------|
| 1 | Single Nano + GY-521 → tilt triggers local LED sequence | ✅ Complete |
| 2 | Two Nanos + wired UART → glove tilt triggers LEDs on a second board | ✅ Complete |
| 3 | Two Nanos + two HC-05 Bluetooth modules → same behavior, wireless | 🔜 Planned |
| 4 | Replace LEDs with motor driver + chassis → full gesture-controlled car | 🔜 Planned |
| 5 | Add ultrasonic obstacle detection as a safety override | 🔜 Planned |

---

## 🎯 How It Works

A glove-mounted Arduino Nano continuously reads tilt angle from a GY-521 (MPU6050) accelerometer/gyroscope module. When the hand tilts left or right past a set threshold, the Nano sends a corresponding command character over a serial link to a second Nano, which drives a set of LEDs in a sequential blink pattern for as long as the gesture is held.

```
[GY-521] --I2C--> [Nano #1 "Glove"] --UART--> [Nano #2 "LED unit"] --digital pins--> [8 LEDs]
```

- Tilt **left** → left-side LEDs blink in sequence (1 → 2 → 3 → 4), repeating while held
- Tilt **right** → right-side LEDs blink in sequence (1 → 2 → 3 → 4), repeating while held
- Return to **neutral** → all LEDs turn off (detected automatically via a communication timeout)

This same command structure (`'L'` / `'R'` over serial) is designed to drop in directly for Bluetooth (Phase 3) and motor control (Phase 4) with minimal code changes.

---

## 🛠️ Hardware Used

- 2 × Arduino Nano
- 1 × GY-521 (MPU6050) accelerometer/gyroscope module
- 8 × LED (4 for left indication, 4 for right indication)
- 8 × 220Ω resistors
- Jumper wires
- Breadboard(s)

**Planned for later phases:** 2 × HC-05 Bluetooth modules, L298N motor driver, DC motors + chassis, HC-SR04 ultrasonic sensor.

---

## 🔌 Wiring

### Nano #1 — Glove Unit
| Component Pin | Nano Pin |
|---|---|
| GY-521 VCC | 5V |
| GY-521 GND | GND |
| GY-521 SCL | A5 |
| GY-521 SDA | A4 |
| UART TX (SoftwareSerial) | Pin 11 → Nano #2 Pin 10 |
| GND | GND → Nano #2 GND |

### Nano #2 — LED Unit
| Component Pin | Nano Pin |
|---|---|
| UART RX (SoftwareSerial) | Pin 10 ← Nano #1 Pin 11 |
| GND | GND ← Nano #1 GND |
| Left LED 1–4 | Pins 2, 3, 4, 5 |
| Right LED 1–4 | Pins 6, 7, 8, 9 |

Each LED: digital pin → 220Ω resistor → LED anode, LED cathode → GND.

See [`docs/wiring.md`](docs/wiring.md) for a full diagram and notes.

---

## 📂 Repository Structure

```
LEDs-Gesture-Link/
├── README.md
├── LICENSE
├── docs/
│   └── wiring.md
└── firmware/
    ├── glove_transmitter/
    │   └── glove_transmitter.ino   # Nano #1 — reads GY-521, sends L/R
    └── led_receiver/
        └── led_receiver.ino        # Nano #2 — receives L/R, drives LEDs
```

---

## 🚀 Getting Started

1. Clone this repo:
   ```bash
   git clone https://github.com/YOUR-USERNAME/LEDs-Gesture-Link.git
   ```
2. Open `firmware/glove_transmitter/glove_transmitter.ino` in the Arduino IDE, select your **Nano #1** board/port, and upload.
3. Open `firmware/led_receiver/led_receiver.ino`, select your **Nano #2** board/port, and upload.
4. Wire both boards according to the [wiring section](#-wiring) above.
5. Power both boards and tilt the glove — the corresponding LED set should blink in sequence.

Open the Serial Monitor (9600 baud) on either board to see debug output.

---

## ⚙️ Configuration

Both sketches expose a few tunable constants near the top of the file:

| Constant | File | Purpose |
|---|---|---|
| `THRESHOLD` | `glove_transmitter.ino` | Degrees of tilt required to trigger a gesture |
| `SEND_INTERVAL` | `glove_transmitter.ino` | How often the command is re-sent while tilted |
| `STEP_DELAY` | `led_receiver.ino` | Speed of the LED sequence |
| `TIMEOUT` | `led_receiver.ino` | How long to wait with no signal before stopping (should be 2–3× `SEND_INTERVAL`) |

---

## 🗺️ Roadmap

- [ ] Swap wired UART for paired HC-05 Bluetooth modules
- [ ] Add pitch (forward/back tilt) detection for forward/backward gestures
- [ ] Replace LED outputs with an L298N motor driver + DC motors
- [ ] Add ultrasonic obstacle detection as a safety override
- [ ] Add a companion mobile app / status LED on the glove

---

## 📄 License

This project is licensed under the MIT License — see [`LICENSE`](LICENSE) for details.

---

## 🙌 Acknowledgments

Built as a learning project exploring IMU sensing, inter-microcontroller communication, and wearable gesture control as a foundation for a gesture-controlled robotics platform.
