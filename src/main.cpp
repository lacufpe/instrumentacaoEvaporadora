// main.cpp - Sistema de Monitoramento de Tanque UR-11
// Arduino UNO R4 WiFi - Código Principal

#include <Arduino.h>
#include <SPI.h>
#include "config.h"
#include "sensor_data.h"
#include "ads1220.h"
#include "sht85.h"
#include "sensor_calibration.h"
#include "connectivity.h"

// ============================================================================
// INSTÂNCIAS GLOBAIS
// ============================================================================

// Dados dos sensores
SensorData g_sensorData;

// ADCs
ADS1220 adc1(ADC1_CS_PIN);
ADS1220 adc2(ADC2_CS_PIN);

// Sensor de temperatura e umidade
SHT85 sht85(SHT85_I2C_ADDR);

// Conectividade
Connectivity connectivity;

// Controle de timing
unsigned long lastSampleTime = 0;

// ============================================================================
// FUNÇÕES AUXILIARES
// ============================================================================

void printBanner() {
  Serial.println(F("==========================================="));
  Serial.println(F("Sistema de Monitoramento de Tanque UR-11"));
  Serial.println(F("Arduino UNO R4 WiFi + MQTT"));
  Serial.println(F("==========================================="));
  Serial.println();
}

void printSensorData() {
  Serial.println(F("╔════════════════════════════════════════╗"));
  Serial.println(F("║    DADOS DOS SENSORES - UR-11          ║"));
  Serial.println(F("╠════════════════════════════════════════╣"));
  
  Serial.print(F("║ Timestamp:     "));
  Serial.print(g_sensorData.timestamp);
  Serial.println(F(" ms"));
  
  Serial.print(F("║ FGT-11 (Vento):     "));
  Serial.print(g_sensorData.windSpeed, 2);
  Serial.println(F(" m/s"));
  
  Serial.print(F("║ LT-11 (Nível):      "));
  Serial.print(g_sensorData.waterLevel, 3);
  Serial.println(F(" m"));
  
  Serial.print(F("║ TT-11 (Prof. 1):    "));
  Serial.print(g_sensorData.temp1, 2);
  Serial.println(F(" °C"));
  
  Serial.print(F("║ TT-12 (Prof. 2):    "));
  Serial.print(g_sensorData.temp2, 2);
  Serial.println(F(" °C"));
  
  Serial.print(F("║ TT-13 (Prof. 3):    "));
  Serial.print(g_sensorData.temp3, 2);
  Serial.println(F(" °C"));
  
  Serial.print(F("║ THT-11 (Temp Ar):   "));
  Serial.print(g_sensorData.airTemp, 2);
  Serial.println(F(" °C"));
  
  Serial.print(F("║ THT-11 (Umidade):   "));
  Serial.print(g_sensorData.humidity, 1);
  Serial.println(F(" %"));
  
  Serial.print(F("║ RT-11 (Radiação):   "));
  Serial.print(g_sensorData.radiation, 1);
  Serial.println(F(" W/m²"));
  
  Serial.println(F("╚════════════════════════════════════════╝"));
  Serial.println();
}

void readAllSensors() {
  // ========================================
  // LEITURA DO ADC1 (Canais 0-3)
  // ========================================
  
  // Canal 0: FGT-11 - Anemômetro
  int32_t raw = adc1.readChannel(0);
  float volt = adc1.toVoltage(raw);
  g_sensorData.windSpeed = convertWindSpeed(volt);
  
  // Canal 1: LT-11 - Nível de água
  raw = adc1.readChannel(1);
  volt = adc1.toVoltage(raw);
  g_sensorData.waterLevel = convertWaterLevel(volt);
  
  // Canal 2: TT-11 - Temperatura 1
  raw = adc1.readChannel(2);
  volt = adc1.toVoltage(raw);
  g_sensorData.temp1 = convertPt100(volt);
  
  // Canal 3: TT-12 - Temperatura 2
  raw = adc1.readChannel(3);
  volt = adc1.toVoltage(raw);
  g_sensorData.temp2 = convertPt100(volt);
  
  // ========================================
  // LEITURA DO ADC2 (Canais 0-1)
  // ========================================
  
  // Canal 0: TT-13 - Temperatura 3
  raw = adc2.readChannel(0);
  volt = adc2.toVoltage(raw);
  g_sensorData.temp3 = convertPt100(volt);
  
  // Canal 1: RT-11 - Radiação solar
  raw = adc2.readChannel(1);
  volt = adc2.toVoltage(raw);
  g_sensorData.radiation = convertRadiation(volt);
  
  // ========================================
  // LEITURA DO SHT85 (I2C)
  // ========================================
  
  if (!sht85.readData(g_sensorData.airTemp, g_sensorData.humidity)) {
    g_sensorData.airTemp = -999.9;
    g_sensorData.humidity = -999.9;
  }
}

// ============================================================================
// SETUP
// ============================================================================

void setup() {
  // Inicializar Serial
  Serial.begin(115200);
  
  unsigned long startTime = millis();
  while (!Serial && (millis() - startTime < 3000)) {
    delay(10);
  }
  
  printBanner();
  
  // Inicializar SPI
  SPI.begin();
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE1));
  Serial.println(F("SPI inicializado"));
  
  // Inicializar ADCs
  Serial.print(F("Inicializando ADC1... "));
  adc1.begin();
  Serial.println(F("OK"));
  
  Serial.print(F("Inicializando ADC2... "));
  adc2.begin();
  Serial.println(F("OK"));
  
  // Inicializar SHT85
  Serial.print(F("Inicializando SHT85... "));
  if (sht85.begin()) {
    Serial.println(F("OK"));
  } else {
    Serial.println(F("FALHA!"));
  }
  
  Serial.println();
  
  // Inicializar conectividade
  connectivity.begin();
  
  Serial.println();
  Serial.println(F("Sistema inicializado!"));
  Serial.println(F("Aquisição a 1 Hz iniciada..."));
  Serial.println();
  
  lastSampleTime = millis();
}

// ============================================================================
// LOOP PRINCIPAL
// ============================================================================

void loop() {
  unsigned long currentTime = millis();
  
  // Atualizar conexões
  connectivity.update();
  
  // Aquisição de dados a 1 Hz
  if (currentTime - lastSampleTime >= SAMPLE_INTERVAL_MS) {
    lastSampleTime = currentTime;
    g_sensorData.timestamp = currentTime;
    
    // Ler todos os sensores
    readAllSensors();
    
    // Enviar dados via MQTT
    connectivity.sendData(g_sensorData);
    
    // Descomentar para ver dados formatados
    // printSensorData();
  }
}