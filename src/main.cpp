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

// ADCs para Pt100 (medição 3 fios)
ADS1220 adcTT11(ADC_TT11_CS_PIN);
ADS1220 adcTT12(ADC_TT12_CS_PIN);
ADS1220 adcTT13(ADC_TT13_CS_PIN);

// Sensor de temperatura e umidade
SHT85 sht85(SHT85_I2C_ADDR);

// Conectividade
Connectivity connectivity;

// Controle de timing
unsigned long lastSampleTime = 0;

// Controle da conexão serial
bool serialConnected = false;

// ============================================================================
// FUNÇÕES AUXILIARES
// ============================================================================

void printBanner() {
  Serial.println(F("==========================================="));
  Serial.println(F("Sistema de Monitoramento de Evaporação UR-11"));
  Serial.println(F("Arduino UNO R4 WiFi + MQTT"));
  Serial.println(F("3x ADS1220 (RTD 3-wire) + ADC interno"));
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
  // LEITURA DOS SENSORES ANALÓGICOS (ADC Arduino)
  // ========================================
  
  // FGT-11 - Anemômetro
  float voltWind = readAnalogVoltage(ANALOG_FGT11_PIN);
  g_sensorData.windSpeed = convertWindSpeed(voltWind);
  
  // LT-11 - Nível de água
  float voltLevel = readAnalogVoltage(ANALOG_LT11_PIN);
  g_sensorData.waterLevel = convertWaterLevel(voltLevel);
  
  // RT-11 - Radiação solar
  float voltRad = readAnalogVoltage(ANALOG_RT11_PIN);
  g_sensorData.radiation = convertRadiation(voltRad);
  
  // ========================================
  // LEITURA DOS Pt100 (ADS1220 com medição 3 fios)
  // ========================================
  
  // TT-11 - Temperatura profundidade 1
  g_sensorData.temp1 = adcTT11.readTemperatureRTD();
  
  // TT-12 - Temperatura profundidade 2
  g_sensorData.temp2 = adcTT12.readTemperatureRTD();
  
  // TT-13 - Temperatura profundidade 3
  g_sensorData.temp3 = adcTT13.readTemperatureRTD();
  
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
  // 3 segundos para aguardar conexão Serial
  while (!Serial && (millis() - startTime < 3000)) {
    delay(10);
  }
  if (Serial) {
    serialConnected = true;
    printBanner();
  }
  
  
  // Configurar resolução do ADC interno
  analogReadResolution(ARDUINO_ADC_RESOLUTION);
  
  if (serialConnected) {  
    Serial.print(F("ADC Arduino configurado: "));
    Serial.print(ARDUINO_ADC_RESOLUTION);
    Serial.println(F(" bits"));
  } 

  // Inicializar SPI
  SPI.begin();
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE1));
  if(serialConnected){Serial.println(F("SPI inicializado"));}
  
  // Inicializar ADCs para Pt100 (medição 3 fios)
  adcTT11.beginRTD3Wire();
  adcTT12.beginRTD3Wire();
  adcTT13.beginRTD3Wire();
  
  // Inicializar SHT85
  if (sht85.begin()) {
    if(serialConnected){Serial.println(F("SHT85 - OK"));}
  } else {
    if(serialConnected){Serial.println(F("SHT85 - Falha"));}
  }
  
  // Inicializar conectividade MQTT
  connectivity.begin();

  if(serialConnected){
    Serial.println();
    Serial.println(F("Sistema inicializado!"));
    Serial.print(F("Aquisição a iniciada. Tempo de amostragem: "));
    Serial.println(SAMPLE_INTERVAL_MS);
    Serial.println();
  }

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
    
    // Envio dos dados via Serial e checagem da conexão serial
    if(serialConnected){
      printSensorData();
      serialConnected = Serial;
    } else {
      if(Serial){
        serialConnected = true;
      } else {
        Serial.begin(115200);
      }
    }
  }
}