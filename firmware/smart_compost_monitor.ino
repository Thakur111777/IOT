/**
 * ============================================================================
 * Project: Smart Soil NPK & Climate Monitoring System (Ergonomic Compost Maker)
 * File:    smart_compost_monitor.ino
 * Target:  Arduino UNO (ATmega328P)
 * ============================================================================
 * DESCRIPTION:
 *   Complete, standalone, production-ready firmware to monitor in-situ soil
 *   macro-nutrients (Nitrogen, Phosphorus, Potassium) and internal compost
 *   chamber microclimate (Temperature & Relative Humidity).
 *
 * HARDWARE PERIPHERALS & INTERFACES:
 *   1. Soil NPK Sensor:
 *      - Protocol: Modbus RTU over RS-485 (Baud rate: 4800 bps, 8-N-1)
 *      - Transceiver: MAX485 TTL-to-RS485 module
 *      - Pins: Arduino D10 (RX), D11 (TX), D7 (DE & RE direction control)
 *      - Registers: Holding register 0x001E (Dec 30), length 3 (N, P, K in mg/kg)
 *      - Power: External regulated 12V DC (shared common ground with Arduino)
 *
 *   2. Climate Sensor (DHT22 / AM2302):
 *      - Interface: Single-bus digital
 *      - Pin: Arduino D2
 *      - Measures: Temperature (-40 to +80 C) and Relative Humidity (0 to 100%)
 *
 *   3. Dual 16x2 Character LCDs (I2C Bus on A4/SDA and A5/SCL):
 *      - Primary Display (Address 0x27): Dedicated to real-time NPK readings
 *      - Secondary Display (Address 0x26): Dedicated to Temperature & Humidity
 *
 * REQUIRED ARDUINO LIBRARIES (Install via Arduino Library Manager):
 *   - "ModbusMaster" by Doc Walker
 *   - "DHT sensor library" by Adafruit
 *   - "LiquidCrystal_I2C" by Frank de Brabander
 * ============================================================================
 */

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <ModbusMaster.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// ==========================================
// PIN & HARDWARE CONFIGURATIONS
// ==========================================

// MAX485 Transceiver Pin Assignments
#define MAX485_DE_RE_PIN   7   // Direction Control (HIGH = Transmit, LOW = Receive)
#define NPK_RX_PIN         10  // Arduino RX <- MAX485 RO
#define NPK_TX_PIN         11  // Arduino TX -> MAX485 DI

// DHT22 Sensor Pin Assignment
#define DHT_PIN            2   // Data pin connected to Arduino D2
#define DHT_TYPE           DHT22

// Dual I2C LCD Addresses
#define LCD_NPK_ADDR       0x27  // Screen 1: NPK Telemetry (Default PCF8574 address)
#define LCD_T_H_ADDR       0x26  // Screen 2: Climate Telemetry (A0 solder jumper bridged)
#define LCD_COLS           16
#define LCD_ROWS           2

// Modbus RTU Settings
#define RS485_BAUD_RATE    4800  // Industrial sensor factory baud rate
#define MODBUS_SLAVE_ID    1     // Sensor default slave address
#define MODBUS_REG_START   30    // Register 30 (0x001E) for Nitrogen
#define MODBUS_REG_COUNT   3     // 3 consecutive registers: N, P, K

// Telemetry & Timing Intervals
#define SERIAL_DEBUG_BAUD  9600  // Serial monitor communication speed
#define SAMPLING_INTERVAL  2000  // Measurement period in milliseconds (2.0s)

// ==========================================
// OBJECT INSTANTIATIONS
// ==========================================

SoftwareSerial rs485(NPK_RX_PIN, NPK_TX_PIN); // RX, TX
ModbusMaster node;
DHT dht(DHT_PIN, DHT_TYPE);

LiquidCrystal_I2C lcdNPK(LCD_NPK_ADDR, LCD_COLS, LCD_ROWS);
LiquidCrystal_I2C lcdTH(LCD_T_H_ADDR, LCD_COLS, LCD_ROWS);

// Global Telemetry Variables
uint16_t N = 0;
uint16_t P = 0;
uint16_t K = 0;
float humidity = 0.0;
float temperature = 0.0;
bool npkSuccess = false;
bool dhtSuccess = false;

unsigned long previousMillis = 0;

// ==========================================
// RS-485 HALF-DUPLEX CALLBACK FUNCTIONS
// ==========================================

void preTransmission() {
  digitalWrite(MAX485_DE_RE_PIN, HIGH); // Set MAX485 to Driver Transmit mode
}

void postTransmission() {
  digitalWrite(MAX485_DE_RE_PIN, LOW);  // Set MAX485 to Receiver Listening mode
}

// ==========================================
// SYSTEM SETUP
// ==========================================

void setup() {
  // Initialize USB Serial Diagnostic Port
  Serial.begin(SERIAL_DEBUG_BAUD);
  while (!Serial && millis() < 1000); // Allow UART settling

  Serial.println(F("\n========================================================"));
  Serial.println(F(" Smart Soil NPK & Composting Monitor - Firmware v1.0    "));
  Serial.println(F("========================================================"));

  // Initialize MAX485 Driver Direction Pin
  pinMode(MAX485_DE_RE_PIN, OUTPUT);
  digitalWrite(MAX485_DE_RE_PIN, LOW); // Start in listening/receive mode

  // Initialize RS485 SoftwareSerial & Modbus Master
  rs485.begin(RS485_BAUD_RATE);
  node.begin(MODBUS_SLAVE_ID, rs485);
  node.preTransmission(preTransmission);
  node.postTransmission(postTransmission);

  // Initialize DHT22 Environmental Sensor
  dht.begin();

  // Initialize I2C Bus & Dual Displays
  Wire.begin();

  // Screen 1: NPK Display Init
  lcdNPK.init();
  lcdNPK.backlight();
  lcdNPK.clear();
  lcdNPK.setCursor(0, 0);
  lcdNPK.print(F("Soil Monitor OK"));
  lcdNPK.setCursor(0, 1);
  lcdNPK.print(F("NPK Screen: 0x27"));

  // Screen 2: Temperature & Humidity Display Init
  lcdTH.init();
  lcdTH.backlight();
  lcdTH.clear();
  lcdTH.setCursor(0, 0);
  lcdTH.print(F("Climate Sens OK"));
  lcdTH.setCursor(0, 1);
  lcdTH.print(F("T/H Screen: 0x26"));

  delay(2000); // Splash screen display window

  lcdNPK.clear();
  lcdTH.clear();
  Serial.println(F("[SYSTEM] All sensors and dual LCD displays initialized successfully."));
  Serial.println(F("[SYSTEM] Entering continuous monitoring loop...\n"));
}

// ==========================================
// MAIN POLLING LOOP
// ==========================================

void loop() {
  unsigned long currentMillis = millis();

  // Non-blocking timer: triggers acquisition every SAMPLING_INTERVAL ms
  if (currentMillis - previousMillis >= SAMPLING_INTERVAL) {
    previousMillis = currentMillis;

    readNPK();
    readDHT();
    updateDisplays();
    printSerialTelemetry();
  }
}

// ==========================================
// SENSOR ACQUISITION SUBROUTINES
// ==========================================

/**
 * @brief Interrogates Soil NPK Sensor holding registers via Modbus RTU
 */
void readNPK() {
  // Query 3 consecutive 16-bit registers starting from register 30
  uint8_t result = node.readHoldingRegisters(MODBUS_REG_START, MODBUS_REG_COUNT);

  if (result == node.ku8MBSuccess) {
    N = node.getResponseBuffer(0); // Nitrogen in mg/kg
    P = node.getResponseBuffer(1); // Phosphorus in mg/kg
    K = node.getResponseBuffer(2); // Potassium in mg/kg
    npkSuccess = true;
  } else {
    npkSuccess = false;
    Serial.print(F("[WARN] Modbus Query Failed! Error Code: 0x"));
    Serial.println(result, HEX);
  }
}

/**
 * @brief Samples ambient compost temperature and humidity from DHT22
 */
void readDHT() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (!isnan(h) && !isnan(t)) {
    humidity = h;
    temperature = t;
    dhtSuccess = true;
  } else {
    dhtSuccess = false;
    Serial.println(F("[WARN] DHT22 Read Failure! Check wiring on digital pin 2."));
  }
}

// ==========================================
// DISPLAY & TELEMETRY SUBROUTINES
// ==========================================

/**
 * @brief Updates both 16x2 I2C LCD screens with latest values
 */
void updateDisplays() {
  // --- Screen 1: NPK Readings ---
  lcdNPK.setCursor(0, 0);
  if (npkSuccess) {
    lcdNPK.print(F("N:"));
    lcdNPK.print(N);
    lcdNPK.print(F("  P:"));
    lcdNPK.print(P);
    lcdNPK.print(F("    ")); // clear trailing characters

    lcdNPK.setCursor(0, 1);
    lcdNPK.print(F("K:"));
    lcdNPK.print(K);
    lcdNPK.print(F(" mg/kg   "));
  } else {
    lcdNPK.print(F("NPK Probe ERR   "));
    lcdNPK.setCursor(0, 1);
    lcdNPK.print(F("Check RS485/12V "));
  }

  // --- Screen 2: Temperature & Humidity Readings ---
  lcdTH.setCursor(0, 0);
  if (dhtSuccess) {
    lcdTH.print(F("Temp: "));
    lcdTH.print(temperature, 1);
    lcdTH.print(F((char)223)); // Degree symbol
    lcdTH.print(F("C   "));

    lcdTH.setCursor(0, 1);
    lcdTH.print(F("Hum:  "));
    lcdTH.print(humidity, 1);
    lcdTH.print(F("%    "));
  } else {
    lcdTH.print(F("DHT Sensor ERR  "));
    lcdTH.setCursor(0, 1);
    lcdTH.print(F("Check Pin 2 Wire"));
  }
}

/**
 * @brief Emits structured telemetry to the Serial Monitor for logging and plotting
 */
void printSerialTelemetry() {
  Serial.print(F("[DATA] Nitrogen: "));
  Serial.print(N);
  Serial.print(F(" mg/kg | Phosphorus: "));
  Serial.print(P);
  Serial.print(F(" mg/kg | Potassium: "));
  Serial.print(K);
  Serial.print(F(" mg/kg | Temp: "));
  Serial.print(temperature, 1);
  Serial.print(F(" C | Humidity: "));
  Serial.print(humidity, 1);
  Serial.println(F(" %"));
}
