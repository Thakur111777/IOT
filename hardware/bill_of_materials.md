# Bill of Materials (BOM)

Detailed breakdown of all hardware components, electrical specifications, communication interfaces, and role within the **Ergonomic Compost Maker (ECM)** system.

---

## 📋 Component List

| Component | Specification / Model | Operating Voltage | Interface / Protocol | Qty | Role in System |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **Microcontroller Board** | Arduino UNO R3 (ATmega328P) | 5V DC (USB / 7-12V Vin) | UART, I2C, SPI, GPIO | 1 | Master controller; executes Modbus master polling, sensor sampling, and dual display driving. |
| **Soil NPK Sensor** | Industrial RS485 Soil Nutrient Transmitter (3-Prong Stainless Steel) | 9V – 24V DC (12V Nominal) | RS-485 Modbus RTU (4800 bps) | 1 | Samples Nitrogen ($N$), Phosphorus ($P$), and Potassium ($K$) ion activity directly inside soil/compost matrix. |
| **Transceiver Module** | MAX485 TTL-to-RS485 Module | 5V DC | TTL UART $\leftrightarrow$ Differential RS485 | 1 | Translates Arduino single-ended TTL signals (`D10`/`D11`) to differential noise-immune RS485 signals ($A$/$B$). |
| **Climate Sensor** | DHT22 / AM2302 High-Accuracy Sensor | 3.3V – 5.5V DC | Single-bus Digital (GPIO `D2`) | 1 | Monitors composting chamber internal temperature (-40 to 80°C $\pm0.5^\circ\text{C}$) and relative humidity (0–100% $\pm2\%$). |
| **Display Unit 1** | 16×2 Character LCD with PCF8574 I2C Backpack | 5V DC | I2C (Address `0x27`) | 1 | Dedicated visual telemetry display for real-time Nitrogen, Phosphorus, and Potassium readings ($mg/kg$). |
| **Display Unit 2** | 16×2 Character LCD with PCF8574 I2C Backpack | 5V DC | I2C (Address `0x26`) | 1 | Dedicated visual telemetry display for composting chamber Temperature (°C) and Humidity (%). |
| **External Power Supply** | 12V 1.5A DC Regulated Wall Adapter | 100–240V AC $\rightarrow$ 12V DC | Barrel Jack / Stripped leads | 1 | Supplies excitation voltage required by industrial NPK probe internal electronics. |
| **Mixing Mechanism** | Rotary Composting Chamber with Motorized Grinder/Agitator | 12V / 230V AC Motor assembly | Mechanical drive | 1 | Provides low-effort ergonomic agitation to homogenize organic waste peels with soil and accelerate microbial aeration. |
| **Prototyping & Enclosure** | Solderless Breadboard, Jumper Wires (M-M, M-F), Custom Chassis | N/A | Mechanical / Physical | 1 | Houses electronics, provides dual-bezel viewing windows for LCDs, and isolates electronics from compost moisture. |

---

## ⚡ Power Distribution Architecture

```
[230V AC Wall Outlet]
        │
        ├───► [12V DC Adapter] ──────┬───► NPK Sensor VCC (Pin 1 / Brown Wire)
        │                            └───► Common Ground Bus (GND)
        │
        └───► [5V USB / DC Adapter] ─┬───► Arduino UNO (5V Bus)
                                     ├───► MAX485 VCC (5V)
                                     ├───► DHT22 VCC (5V)
                                     ├───► Dual I2C LCD Backlights & VCC (5V)
                                     └───► Common Ground Bus (GND) [CRITICAL]
```

> [!IMPORTANT]
> **Common Ground Requirement:** The 12V external power supply ground and the Arduino 5V ground **must be tied together**. A floating ground reference causes RS485 communication failures and erratic register reads.
