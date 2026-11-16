# Firmware Documentation & Guide

This directory contains the embedded source code for the **Ergonomic Compost Maker (ECM)** system.

---

## 📂 Directory Structure

| File / Folder | Purpose |
| :--- | :--- |
| [`config.h`](./config.h) | Centralized system constants, pin definitions, Modbus register IDs, and I2C addresses. |
| [`smart_compost_monitor/`](./smart_compost_monitor/) | Main Arduino UNO firmware responsible for Modbus RTU communication, DHT22 acquisition, and dual LCD rendering. |

---

## 🛠️ Required Arduino Libraries

Ensure the following libraries are installed through the **Arduino IDE Library Manager** (`Sketch` -> `Include Library` -> `Manage Libraries...`):

1. **`ModbusMaster`** (by Doc Walker) – Handles the Modbus RTU protocol over serial to interrogate the Soil NPK sensor.
2. **`DHT sensor library`** (by Adafruit) – Decodes digital single-bus signals from the DHT22 temperature and humidity sensor.
3. **`Adafruit Unified Sensor`** – Prerequisite core library for Adafruit sensor drivers.
4. **`LiquidCrystal_I2C`** (by Frank de Brabander / Marco Schwartz) – Facilitates I2C command control of HD44780-compatible 16x2 LCD modules.
5. **`SoftwareSerial`** & **`Wire`** – Standard built-in AVR libraries included with the Arduino core.

---

## ⚙️ Hardware Communication Protocols

### 1. Modbus RTU over RS485 (Soil NPK Sensor)
* **Baud Rate:** `4800 bps` (8 Data bits, 1 Stop bit, No Parity).
* **Direction Switching:** Controlled via Arduino Pin `D7` connected to MAX485 `DE` and `RE` pins.
  * `HIGH` = Arduino transmits inquiry request.
  * `LOW` = Arduino listens for sensor response frames.
* **Modbus Register Address:** Holding register `0x001E` (Decimal `30`). Three consecutive 16-bit registers contain Nitrogen ($N$), Phosphorus ($P$), and Potassium ($K$) concentrations in $mg/kg$.

### 2. Dual I2C Bus Addressing
Both 16x2 LCD screens share Arduino pins `A4` (SDA) and `A5` (SCL). To prevent bus collisions, separate addresses were configured on the PCF8574 backpack:
* **Primary Screen (NPK Readout):** Default address `0x27` (all solder jumpers open).
* **Secondary Screen (Temperature & Humidity):** Address `0x26` (solder jumper `A0` bridged to ground).

---

## 🚀 Flashing & Upload Instructions

1. Connect the **Arduino UNO** to your computer via USB cable.
2. Open [`smart_compost_monitor/smart_compost_monitor.ino`](./smart_compost_monitor/smart_compost_monitor.ino) in Arduino IDE.
3. Select board: `Tools` -> `Board` -> `Arduino AVR Boards` -> `Arduino Uno`.
4. Select the corresponding serial port: `Tools` -> `Port` -> `COMx`.
5. Click **Verify (Ctrl + R)** to ensure compilation passes without errors.
6. Click **Upload (Ctrl + U)**.
7. Open **Serial Monitor** at **9600 Baud** to verify startup diagnostic logs.

---

## 🔍 Diagnostic & Troubleshooting Checklist

| Symptom | Probable Cause | Corrective Action |
| :--- | :--- | :--- |
| **NPK displays 0-0-0** | Sensor is in open air or soil is completely dry. | Insert probe fully into moist soil mixture; ensure minimum 20% soil moisture for conductivity. |
| **Modbus Read Error 0xE2 (Timeout)** | Reversed RS485 A/B lines or missing 12V supply. | Verify MAX485 `A` connects to Sensor `Yellow (A)`, `B` to Sensor `Blue (B)`. Confirm the NPK sensor is powered with an external 12V DC adapter. |
| **One or both LCDs blank** | I2C address mismatch or contrast potentiometer unadjusted. | Adjust the blue potentiometer on the rear of each I2C backpack. Run an I2C scanner sketch to verify addresses `0x27` and `0x26`. |
| **DHT22 returns NaN** | Missing pull-up resistor or loose pin `D2`. | Check jumper on digital pin 2; ensure supply voltage is stable at 5V. |
