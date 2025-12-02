// ads1220.cpp - Implementação do driver ADS1220
#include "ads1220.h"
#include "config.h"
#include <math.h>

ADS1220::ADS1220(uint8_t csPin) : _csPin(csPin) {}

void ADS1220::begin() {
  pinMode(_csPin, OUTPUT);
  digitalWrite(_csPin, HIGH);
  reset();
}

void ADS1220::beginRTD3Wire() {
  pinMode(_csPin, OUTPUT);
  digitalWrite(_csPin, HIGH);
  reset();
  
  // Configuração para medição RTD 3 fios
  // REG0: MUX[3:0]=0000 (AIN0-AIN1 para medição diferencial)
  //       GAIN[2:0]=011 (Gain=8), PGA_BYPASS=0
  //       00000110
  writeRegister(0x00, 0x06);
  
  // REG1: DR[2:0]=000 (20 SPS para reduzir ruído)
  //       MODE[1:0]=00 (Normal mode)
  //       CM=0 (Continuous conversion mode desabilitado)
  //       TS=0 (Temp sensor desabilitado)
  //       BCS=0 (Burn-out current desabilitado)
  //      00000000
  writeRegister(0x01, 0x00);
  
  // REG2: VREF[1:0]=00 (Referência interna de 2,048 V)
  //       FIR[1:0]=11 (rejeição de 60 Hz)
  //       PSW=0 (Low-side power switch aberto)
  //       IDAC[2:0]=111 (IDAC = 1500µA para excitação do RTD)
  //       00110111
  writeRegister(0x02, 0x37);
  
  // REG3: I1MUX[2:0]=011 (IDAC1 para AIN2)
  //       I2MUX[2:0]=100 (IDAC2 para AIN3)
  //       DRDYM=0 (DRDY apenas no pino)
  //      01110000
  writeRegister(0x03, 0x70);
  
  delay(100);
}

void ADS1220::reset() {
  digitalWrite(_csPin, LOW);
  delayMicroseconds(10);
  SPI.transfer(ADS1220_CMD_RESET);
  digitalWrite(_csPin, HIGH);
  delay(10);
}

void ADS1220::writeRegister(uint8_t reg, uint8_t value) {
  digitalWrite(_csPin, LOW);
  delayMicroseconds(10);
  SPI.transfer(ADS1220_CMD_WREG | (reg << 2));
  SPI.transfer(value);
  digitalWrite(_csPin, HIGH);
  delayMicroseconds(100);
}

uint8_t ADS1220::readRegister(uint8_t reg) {
  digitalWrite(_csPin, LOW);
  delayMicroseconds(10);

  // 0100rr00
  SPI.transfer(ADS1220_CMD_RREG | ((reg&0x03) << 2));
  uint8_t value = SPI.transfer(0x00);
  digitalWrite(_csPin, HIGH);
  return value;
}

int32_t ADS1220::readRaw() {
  // Iniciar conversão
  digitalWrite(_csPin, LOW);
  delayMicroseconds(10);
  SPI.transfer(ADS1220_CMD_START);
  digitalWrite(_csPin, HIGH);
  
  delay(100); // Aguardar conversão (20 SPS = 50ms + margem)
  
  // Ler dados (24 bits)
  digitalWrite(_csPin, LOW);
  delayMicroseconds(10);
  SPI.transfer(ADS1220_CMD_RDATA);
  int32_t data = 0;
  data = (int32_t)SPI.transfer(0x00) << 16;
  data |= (int32_t)SPI.transfer(0x00) << 8;
  data |= SPI.transfer(0x00);
  digitalWrite(_csPin, HIGH);
  
  // Converter para complemento de 2
  if (data & 0x800000) {
    data |= 0xFF000000;
  }
  
  return data;
}

float ADS1220::toVoltage(int32_t raw) {
  // Com Gain=8 e Vref=2.048V
  // Tensão = (raw * Vref) / (Gain * 2^23)
  return (raw * 2.048) / (8.0 * 8388608.0);
}

float ADS1220::resistanceToTemperature(float resistance) {
  // Equação de Callendar-Van Dusen simplificada (linear)
  // T = (R - R0) / (R0 * α)
  // Para maior precisão, usar polinômio completo
  
  float temp = PT100_K1 - sqrtf(PT100_K2 + PT100_k3 * resistance);
  
  return temp;
}

float ADS1220::readTemperatureRTD() {
  // Ler tensão diferencial
  int32_t raw = readRaw();
  float voltage = toVoltage(raw);
  
  // Calcular resistência do RTD
  // V = I * R
  // R = V / I
  // Com IDAC = 1500µA = 0.0015A
  float resistance = voltage / PT100_IDAC_CURRENT;
  
  // Converter resistência para temperatura
  float temperature = resistanceToTemperature(resistance);
  
  return temperature;
}