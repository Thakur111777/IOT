# Complete Pinout & Wiring Connections

Comprehensive interconnectivity mapping between the **Arduino UNO**, **MAX485 Transceiver**, **Soil NPK Sensor**, **DHT22 Sensor**, and **Dual I2C 16x2 Displays**.

---

## 🔌 1. Arduino UNO $\leftrightarrow$ MAX485 TTL-to-RS485 Module

| Arduino UNO Pin | MAX485 Module Pin | Function | Description |
| :--- | :--- | :--- | :--- |
| **5V** | `VCC` | Power | 5V DC Supply rail |
| **GND** | `GND` | Power | System Ground reference |
| **Pin 10** | `RO` (Receiver Output) | Serial RX | SoftwareSerial RX (Receives data from MAX485) |
| **Pin 11** | `DI` (Driver Input) | Serial TX | SoftwareSerial TX (Sends commands to MAX485) |
| **Pin 7** | `DE` (Driver Enable) | Control | Transmit control line (Active `HIGH`) |
| **Pin 7** | `RE` (Receiver Enable) | Control | Receive control line (Active `LOW`) |

> *Note:* `DE` and `RE` on the MAX485 are jumpered together and driven by Arduino digital pin `7`.

---

## 🌾 2. MAX485 Module $\leftrightarrow$ Industrial Soil NPK Sensor

| MAX485 Terminal | NPK Sensor Wire Color | Modbus Line | Voltage / Purpose |
| :--- | :--- | :--- | :--- |
| **A** | **Yellow** | `A` / `RS485+` | Non-inverting differential data line |
| **B** | **Blue** | `B` / `RS485-` | Inverting differential data line |
| *(External 12V+)* | **Brown** | `VCC` | External +12V DC Regulated Supply |
| **GND (Common)** | **Black** | `GND` | Common Ground (Shared with Arduino GND) |

---

## 🌡️ 3. Arduino UNO $\leftrightarrow$ DHT22 Temperature & Humidity Sensor

| Arduino UNO Pin | DHT22 Pin | Description |
| :--- | :--- | :--- |
| **5V** | Pin 1 (`VCC`) | 5V DC Power rail |
| **Pin 2** | Pin 2 (`DATA`) | Bidirectional Single-Bus Digital Data line |
| *NC* | Pin 3 (`NC`) | No Connection / Unused |
| **GND** | Pin 4 (`GND`) | System Ground reference |

---

## 📺 4. Arduino UNO $\leftrightarrow$ Dual 16x2 I2C LCD Displays

Both displays share the hardware I2C bus on the Arduino UNO. Address differentiation is achieved using solder pads on the PCF8574 I2C adapter board.

| Arduino UNO Pin | Primary LCD (NPK) | Secondary LCD (Temp & Humidity) | Description |
| :--- | :--- | :--- | :--- |
| **5V** | `VCC` | `VCC` | Shared 5V Power rail |
| **GND** | `GND` | `GND` | Shared Ground rail |
| **A4** | `SDA` | `SDA` | I2C Serial Data Bus |
| **A5** | `SCL` | `SCL` | I2C Serial Clock Bus |
| **I2C Address** | **`0x27`** (Default) | **`0x26`** (`A0` Jumper Bridged) | Bus Device Identification |

---

## 📐 Circuit Schematic Reference

Refer to the high-resolution architectural wiring schematic located in [`pictures/circuit_schematic.png`](../pictures/circuit_schematic.png).
