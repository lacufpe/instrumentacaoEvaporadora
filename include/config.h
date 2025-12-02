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
// Pinos CS dos ADS1220 (um para cada Pt100)
#define ADC_TT11_CS_PIN 10  // ADS1220 para TT-11
#define ADC_TT12_CS_PIN 9   // ADS1220 para TT-12
#define ADC_TT13_CS_PIN 8   // ADS1220 para TT-13

// Pinos analógicos do Arduino R4
#define ANALOG_FGT11_PIN A0  // Anemômetro
#define ANALOG_LT11_PIN  A1  // Sensor de nível
#define ANALOG_RT11_PIN  A2  // Piranômetro

// Endereço I2C do SHT85
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
// CONFIGURAÇÃO ADC ARDUINO
// ============================================================================
#define ARDUINO_ADC_RESOLUTION 14  // Arduino R4 tem ADC de 14 bits
#define ARDUINO_ADC_VREF 5.0       // Tensão de referência (5V)
#define ARDUINO_ADC_MAX_VALUE 16383.0  // 2^14 - 1

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

// Piranômetro RT-11 (SN300AL)
#define RADIATION_VOLTAGE_MIN 0.0
#define RADIATION_VOLTAGE_MAX 5.0
#define RADIATION_MIN 0.0
#define RADIATION_MAX 2000.0  // W/m²

// Pt100 - Parâmetros RTD
// Pt100: R0 = 100Ω, A=3.9083×10−3°C−1 e B=−5.775×10–7°C–2 .
// R = R0*(1 + A*T + B*T²) => T = (-A + sqrt(A² - 4*B*(1 - R/R0)))/(2*B)
// T = k1 - sqrt(k2 + k3*R)
#define PT100_K1 1.1285216249999998e-09
#define PT100_K2 13181768.62502577
#define PT100_k3 -17316.017316017318

// Configuração de corrente de excitação do ADS1220 para RTD
#define PT100_IDAC_CURRENT 0.0015  // A (ajustar conforme necessário)

#endif // CONFIG_H