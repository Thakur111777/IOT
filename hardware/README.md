# Hardware Architecture & Design Overview

This folder contains hardware schematics, interfacing documentation, power distribution schematics, and component specifications for the **Ergonomic Compost Maker (ECM)**.

---

## 🏗️ Hardware Architecture Principles

1. **Noise-Immune Differential Signaling (RS-485):**
   Compost matrices introduce high electromagnetic noise, damp conditions, and long wire runs. Conventional TTL UART or I2C lines cannot safely traverse such hostile conditions without data corruption. The ECM utilizes differential RS-485 signaling governed by Modbus RTU, ensuring dependable register reads under all ambient moisture conditions.

2. **Dual-Voltage Domain Isolation:**
   - **5V Domain:** Powers low-voltage digital logic (ATmega328P, DHT22 sensor, MAX485 driver, and LCD character displays).
   - **12V Domain:** Powers internal excitation coils and capacitive electrodes in the industrial-grade NPK probe.
   - Both domains share an unified low-impedance ground return plane to eliminate ground loop voltage offsets.

3. **Ergonomic Mechanical Agitation:**
   Unlike traditional composting pits requiring heavy pitchfork manual labor, ECM embeds a compact rotary mixing grinder assembly. This allows fine grinding and homogenization of tough organic peels (e.g., banana, citrus, kitchen scraps) directly with soil, increasing the specific surface area available for aerobic microbial digestion.

---

## 📂 Documentation Directory

* [`bill_of_materials.md`](./bill_of_materials.md) – Detailed BOM, voltage specifications, component roles, and procurement details.
* [`pinout_and_connections.md`](./pinout_and_connections.md) – Full wiring tables and pin assignments.
* Schematic diagram: [`pictures/circuit_schematic.png`](../pictures/circuit_schematic.png).
