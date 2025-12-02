// sensor_calibration.h - Funções de calibração dos sensores
#ifndef SENSOR_CALIBRATION_H
#define SENSOR_CALIBRATION_H

#include <Arduino.h>

// ============================================================================
// FUNÇÕES DE LEITURA DO ADC ARDUINO
// ============================================================================

// Lê entrada analógica e converte para tensão
float readAnalogVoltage(uint8_t pin);

// ============================================================================
// FUNÇÕES DE CONVERSÃO E CALIBRAÇÃO
// ============================================================================

// Converte tensão do anemômetro para velocidade do vento
float convertWindSpeed(float voltage);

// Converte tensão do sensor de nível para altura da água
float convertWaterLevel(float voltage);

// Converte tensão do piranômetro para radiação solar
float convertRadiation(float voltage);

#endif // SENSOR_CALIBRATION_H