/**
 * @file config.h
      * @brief System Configuration & Pin Definitions for Ergonomic Compost
      * Maker (ECM)
      * @author Lucky Walia, Pranav Sharma, Purujeet Singh Thakur
      * @course Cloud-Integrated IoT Systems [25EE036]
      */

#ifndef CONFIG_H
#define CONFIG_H

// ==========================================
// PIN DEFINITIONS
// ==========================================

// MAX485 TTL-to-RS485 Transceiver Pins
#define MAX485_DE_RE_PIN 7 // Driver Enable & Receiver Enable (tied together)
#define NPK_RX_PIN 10      // Arduino RX <- MAX485 RO
#define NPK_TX_PIN 11      // Arduino TX -> MAX485 DI

// DHT22 Temperature & Humidity Sensor
#define DHT_PIN 2 // Digital Pin connected to DHT22 data pin
#define DHT_TYPE DHT22

// ==========================================
// I2C LCD CONFIGURATIONS
// ==========================================

// Dual 16x2 LCD I2C Addresses (A0/A1/A2 solder jumpers configured on PCF8574
// backpack)
#define LCD_NPK_ADDR 0x27 // Primary LCD for NPK nutrient levels
#define LCD_T_H_ADDR 0x26 // Secondary LCD for Temperature and Humidity
#define LCD_COLS 16
#define LCD_ROWS 2

// ==========================================
// MODBUS RS485 SENSOR CONFIGURATIONS
// ==========================================

#define RS485_BAUD_RATE 4800 // Industrial soil NPK sensor baud rate
#define MODBUS_SLAVE_ID 1    // Default Modbus RTU Slave ID
#define MODBUS_REG_START 30  // Starting holding register address for NPK
#define MODBUS_REG_COUNT 3   // Read 3 consecutive registers (N, P, K)

// ==========================================
// SYSTEM TIMING & TELEMETRY SETTINGS
// ==========================================

#define SERIAL_DEBUG_BAUD 9600  // Serial monitor debug baud rate
#define TELEMETRY_INTERVAL 2000 // Sampling interval in milliseconds (2 seconds)
#define STARTUP_DELAY_MS 2000   // Splash screen delay

#endif // CONFIG_H
