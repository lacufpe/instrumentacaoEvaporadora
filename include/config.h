// config.h - Configurações Gerais do Sistema UR-11
#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// ============================================================================
// CONFIGURAÇÕES WIFI
// ============================================================================
#define WIFI_SSID "SUA_REDE_WIFI"
#define WIFI_PASS "SUA_SENHA_WIFI"

// ============================================================================
// CONFIGURAÇÕES MQTT
// ============================================================================
#define MQTT_BROKER "broker.hivemq.com"
#define MQTT_PORT 1883
#define MQTT_TOPIC "ur11/tank/data"
#define MQTT_CLIENT_ID "UR11_Arduino_R4"
#define MQTT_USER ""  // Vazio se não usar autenticação
#define MQTT_PASS ""

// ============================================================================
// CONFIGURAÇÕES DE HARDWARE - PINOS
// ============================================================================
#define ADC1_CS_PIN 10  // ADS1220 #1
#define ADC2_CS_PIN 9   // ADS1220 #2
#define SHT85_I2C_ADDR 0x44

// ============================================================================
// CONFIGURAÇÕES DE TIMING
// ============================================================================
#define SAMPLE_INTERVAL_MS 1000  // 1 Hz
#define RECONNECT_INTERVAL_MS 5000

// ============================================================================
// COMANDOS ADS1220
// ============================================================================
#define ADS1220_CMD_RESET 0x06
#define ADS1220_CMD_START 0x08
#define ADS1220_CMD_RDATA 0x10
#define ADS1220_CMD_RREG  0x20
#define ADS1220_CMD_WREG  0x40

// ============================================================================
// CALIBRAÇÃO DOS SENSORES
// ============================================================================
// Anemômetro FGT-11
#define WIND_VOLTAGE_MIN 0.0
#define WIND_VOLTAGE_MAX 5.0
#define WIND_SPEED_MIN 0.0
#define WIND_SPEED_MAX 50.0  // m/s

// Sensor de Nível LT-11
#define LEVEL_VOLTAGE_MIN 1.0  // 4mA
#define LEVEL_VOLTAGE_MAX 5.0  // 20mA
#define LEVEL_HEIGHT_MIN 0.0
#define LEVEL_HEIGHT_MAX 10.0  // metros

// Pt100 (TT-11, TT-12, TT-13)
#define PT100_VOLTAGE_MIN 0.0
#define PT100_VOLTAGE_MAX 5.0
#define PT100_TEMP_MIN 0.0
#define PT100_TEMP_MAX 100.0  // °C

// Piranômetro RT-11 (SN300AL)
#define RADIATION_VOLTAGE_MIN 0.0
#define RADIATION_VOLTAGE_MAX 5.0
#define RADIATION_MIN 0.0
#define RADIATION_MAX 2000.0  // W/m²

#endif // CONFIG_H