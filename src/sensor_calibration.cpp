// sensor_calibration.cpp - Implementação das calibrações
#include "sensor_calibration.h"
#include "config.h"

float convertWindSpeed(float voltage) {
  if (voltage < WIND_VOLTAGE_MIN) voltage = WIND_VOLTAGE_MIN;
  if (voltage > WIND_VOLTAGE_MAX) voltage = WIND_VOLTAGE_MAX;
  
  // Conversão linear: y = mx + b
  float range_v = WIND_VOLTAGE_MAX - WIND_VOLTAGE_MIN;
  float range_s = WIND_SPEED_MAX - WIND_SPEED_MIN;
  
  return WIND_SPEED_MIN + (voltage - WIND_VOLTAGE_MIN) * (range_s / range_v);
}

float convertWaterLevel(float voltage) {
  if (voltage < LEVEL_VOLTAGE_MIN) voltage = LEVEL_VOLTAGE_MIN;
  if (voltage > LEVEL_VOLTAGE_MAX) voltage = LEVEL_VOLTAGE_MAX;
  
  // Conversão linear 4-20mA (1-5V)
  float range_v = LEVEL_VOLTAGE_MAX - LEVEL_VOLTAGE_MIN;
  float range_h = LEVEL_HEIGHT_MAX - LEVEL_HEIGHT_MIN;
  
  return LEVEL_HEIGHT_MIN + (voltage - LEVEL_VOLTAGE_MIN) * (range_h / range_v);
}

float convertPt100(float voltage) {
  if (voltage < PT100_VOLTAGE_MIN) voltage = PT100_VOLTAGE_MIN;
  if (voltage > PT100_VOLTAGE_MAX) voltage = PT100_VOLTAGE_MAX;
  
  // Conversão linear simplificada
  // NOTA: Para maior precisão, implementar polinômio ou tabela de lookup
  float range_v = PT100_VOLTAGE_MAX - PT100_VOLTAGE_MIN;
  float range_t = PT100_TEMP_MAX - PT100_TEMP_MIN;
  
  return PT100_TEMP_MIN + (voltage - PT100_VOLTAGE_MIN) * (range_t / range_v);
}

float convertRadiation(float voltage) {
  if (voltage < RADIATION_VOLTAGE_MIN) voltage = RADIATION_VOLTAGE_MIN;
  if (voltage > RADIATION_VOLTAGE_MAX) voltage = RADIATION_VOLTAGE_MAX;
  
  // Conversão linear baseada na calibração do piranômetro
  float range_v = RADIATION_VOLTAGE_MAX - RADIATION_VOLTAGE_MIN;
  float range_r = RADIATION_MAX - RADIATION_MIN;
  
  return RADIATION_MIN + (voltage - RADIATION_VOLTAGE_MIN) * (range_r / range_v);
}