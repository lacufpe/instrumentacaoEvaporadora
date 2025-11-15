// sensor_data.h - Estrutura de Dados dos Sensores
#ifndef SENSOR_DATA_H
#define SENSOR_DATA_H

#include <Arduino.h>

// ============================================================================
// ESTRUTURA DE DADOS DOS SENSORES
// ============================================================================
struct SensorData {
  float windSpeed;      // FGT-11 - Velocidade do vento (m/s)
  float waterLevel;     // LT-11 - Nível de água (m)
  float temp1;          // TT-11 - Temperatura profundidade 1 (°C)
  float temp2;          // TT-12 - Temperatura profundidade 2 (°C)
  float temp3;          // TT-13 - Temperatura profundidade 3 (°C)
  float airTemp;        // THT-11 - Temperatura do ar (°C)
  float humidity;       // THT-11 - Umidade relativa (%)
  float radiation;      // RT-11 - Radiação solar (W/m²)
  unsigned long timestamp;
};

// Instância global
extern SensorData g_sensorData;

#endif // SENSOR_DATA_H