# Wiring Guide

## System Overview

```
              I2C                 UART (SoftwareSerial)
[GY-521] <----------> [Nano #1] <----------------------> [Nano #2] ----> [8x LED]
                       "Glove"          Pin 11 -> Pin 10    "LED Unit"
                                         GND    -> GND
```

## Nano #1 — Glove Unit

### GY-521 (MPU6050) — I2C connection

| GY-521 Pin | Nano Pin |
|---|---|
| VCC | 5V |
| GND | GND |
| SCL | A5 |
| SDA | A4 |
| XDA | Not connected |
| XCL | Not connected |
| AD0 | Not connected |
| INT | Not connected |

### UART link to Nano #2

| Nano #1 Pin | Connects To |
|---|---|
| Pin 11 (SoftwareSerial TX) | Nano #2 Pin 10 (SoftwareSerial RX) |
| GND | Nano #2 GND |

> ⚠️ The shared GND connection is required. Without a common ground reference, the UART signal will be unreliable or won't work at all.

---

## Nano #2 — LED Unit

### UART link from Nano #1

| Nano #2 Pin | Connects To |
|---|---|
| Pin 10 (SoftwareSerial RX) | Nano #1 Pin 11 (SoftwareSerial TX) |
| GND | Nano #1 GND |

### LEDs

Each LED is wired: **digital pin → 220Ω resistor → LED anode (long leg) → LED cathode (short leg) → GND**

| LED Group | Pins |
|---|---|
| Left indicator (1–4) | 2, 3, 4, 5 |
| Right indicator (1–4) | 6, 7, 8, 9 |

---

## Future: Swapping UART Wire for Bluetooth (Phase 3)

Both HC-05 modules will connect using the **same pins (10 and 11)** already reserved for `SoftwareSerial` on both boards:

| HC-05 Pin | Nano Pin |
|---|---|
| VCC | 5V |
| GND | GND |
| TXD | Pin 10 |
| RXD | Pin 11 |

Once the HC-05 modules are configured (one as Master, one as Slave) and paired, they replace the physical wire entirely — **no firmware changes required**, since the sketches already communicate through `SoftwareSerial` rather than the hardware UART pins.
