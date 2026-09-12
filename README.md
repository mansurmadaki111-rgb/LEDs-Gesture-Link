# LEDs-Gesture-Link

**A wearable hand-gesture control system built with Arduino Nano and an MPU6050 (GY-521) IMU.**

LEDs-Gesture-Link reads hand tilt from a glove-mounted motion sensor and wirelessly triggers actions on a receiving unit — currently an LED indicator array, with a gesture-controlled obstacle-avoidance robot car planned as the end goal.

---

## 📌 Project Status

This repo is being built in phases. Current phase: **Phase 3 — Wireless Bluetooth link between two Nanos.**

| Phase | Description | Status |
|-------|-------------|--------|
| 1 | Single Nano + GY-521 → tilt triggers local LED sequence | ✅ Complete |
| 2 | Two Nanos + wired UART → glove tilt triggers LEDs on a second board | ✅ Complete |
| 3 | Two Nanos + two HC-05 Bluetooth modules → same behavior, wireless | ✅ Complete |
| 4 | Replace LEDs with motor driver + chassis → full gesture-controlled car | 🔜 Planned |
| 5 | Add ultrasonic obstacle detection as a safety override | 🔜 Planned |

---

## 🎯 How It Works

A glove-mounted Arduino Nano continuously reads tilt angle from a GY-521 (MPU6050) accelerometer/gyroscope module. When the hand tilts left or right past a set threshold, the Nano sends a corresponding command character to a second Nano, which drives a set of LEDs in a sequential blink pattern for as long as the gesture is held.

```
[GY-521] --I2C--> [Nano #1 "Glove"] --link--> [Nano #2 "LED unit"] --digital pins--> [8 LEDs]
```

- Tilt **left** → left-side LEDs blink in sequence (1 → 2 → 3 → 4), repeating while held
- Tilt **right** → right-side LEDs blink in sequence (1 → 2 → 3 → 4), repeating while held
- Return to **neutral** → all LEDs turn off (detected automatically via a communication timeout)

This same command structure (`'L'` / `'R'`) drops in directly for motor control (Phase 4) with minimal code changes — only the receiver's response to each command changes, not the communication logic itself.

---

## 🔀 Choose Your Connection Method

The `"link"` between the two Nanos shown above can be either a **plain wire** or a pair of **Bluetooth modules** — the firmware is identical either way, since both sketches communicate through `SoftwareSerial` on pins 10/11 without caring what's physically attached to them.

If you only have two Arduino Nanos and no Bluetooth modules yet, start with the wired version — it's cheaper, has zero setup, and lets you confirm the gesture logic works before adding wireless complexity.

| | Wired UART | Bluetooth (HC-05) |
|---|---|---|
| **Hardware needed** | 2 × Arduino Nano, 2 jumper wires | 2 × Arduino Nano, 2 × HC-05 module |
| **Setup required** | None — just wire pin 11 → pin 10, GND → GND | One-time AT command pairing (Master/Slave) |
| **Range** | A few centimeters (must be physically wired together) | Several meters, fully wireless |
| **Firmware used** | `glove_transmitter.ino` / `led_receiver.ino` (unmodified) | Same exact files, no changes |
| **Best for** | Quick testing, confirming gesture logic works | The actual wearable glove use case |
| **Guide** | See [wiring table below](#-wiring) | See [`docs/bluetooth_setup.md`](docs/bluetooth_setup.md) |

**In short:** the code never changes — only what's physically connected to pins 10/11 does. Start wired, confirm everything works, then swap in Bluetooth modules with zero firmware edits once you're ready to go wireless.

---

## 🛠️ Hardware Used

- 2 × Arduino Nano
- 1 × GY-521 (MPU6050) accelerometer/gyroscope module
- 2 × HC-05 Bluetooth module (one Master, one Slave) — *optional, only needed for the wireless connection method above*
- 8 × LED (4 for left indication, 4 for right indication)
- 8 × 220Ω resistors
- Jumper wires
- Breadboard(s)

**Planned for later phases:** L298N motor driver, DC motors + chassis, HC-SR04 ultrasonic sensor.

---

## 🔌 Wiring

### Nano #1 — Glove Unit
| Component Pin | Nano Pin |
|---|---|
| GY-521 VCC | 5V |
| GY-521 GND | GND |
| GY-521 SCL | A5 |
| GY-521 SDA | A4 |
| Link TX (SoftwareSerial) | Pin 11 → Nano #2 Pin 10 (wire), or HC-05 Master TXD/RXD (Bluetooth) |
| GND | GND → Nano #2 GND |

### Nano #2 — LED Unit
| Component Pin | Nano Pin |
|---|---|
| Link RX (SoftwareSerial) | Pin 10 ← Nano #1 Pin 11 (wire), or HC-05 Slave TXD/RXD (Bluetooth) |
| GND | GND ← Nano #1 GND |
| Left LED 1–4 | Pins 2, 3, 4, 5 |
| Right LED 1–4 | Pins 6, 7, 8, 9 |

Each LED: digital pin → 220Ω resistor → LED anode, LED cathode → GND.

See [`docs/wiring.md`](docs/wiring.md) for the full wired-connection diagram, and [`docs/bluetooth_setup.md`](docs/bluetooth_setup.md) for HC-05 pairing steps if going wireless.

---

## 📸 Build Photos

### 2-Nano Wired Connection

<img src="2nano's/LEDs-Gesture-Link.jpg" width="400">
<img src="2nano's/LEDs-Gesture-Link1.jpg" width="400">
<img src="2nano's/LEDs-Gesture-Link2.jpg" width="400">

### Bluetooth Module Connection

<img src="BT%20images%20connection/BT%20module.jpg" width="400">
<img src="BT%20images%20connection/BT%20module2.jpg" width="400">

> ⚠️ **Note:** the photos above show the Bluetooth (HC-05) wiring only — the LEDs were **not** physically connected at this stage. This phase was verified entirely through the Serial Monitor (see [`docs/bluetooth_setup.md`](docs/bluetooth_setup.md)), confirming the wireless link and gesture logic worked correctly before the LEDs were wired back in.

---

## 📂 Repository Structure

```
LEDs-Gesture-Link/
├── README.md
├── LICENSE
├── docs/
│   ├── wiring.md
│   └── bluetooth_setup.md
├── 2nano's/
│   ├── LEDs-Gesture-Link.jpg
│   ├── LEDs-Gesture-Link1.jpg
│   └── LEDs-Gesture-Link2.jpg
├── BT images connection/
│   ├── BT module.jpg
│   └── BT module2.jpg
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
   git clone https://github.com/mansurmadaki111-rgb/LEDs-Gesture-Link.git
   ```
2. Open `firmware/glove_transmitter/glove_transmitter.ino` in the Arduino IDE, select your **Nano #1** board/port, and upload.
3. Open `firmware/led_receiver/led_receiver.ino`, select your **Nano #2** board/port, and upload.
4. Choose your connection method (see [table above](#-choose-your-connection-method)):
   - **Wired:** connect pin 11 (Nano #1) → pin 10 (Nano #2), plus GND → GND
   - **Bluetooth:** pair two HC-05 modules following [`docs/bluetooth_setup.md`](docs/bluetooth_setup.md), then wire them to pins 10/11 on their respective boards
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

- [x] Swap wired UART for paired HC-05 Bluetooth modules
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

---

## 👤 Author

**Mansur Isah**
