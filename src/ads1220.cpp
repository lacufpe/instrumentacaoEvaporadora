// ads1220.cpp - Implementação do driver ADS1220
#include "ads1220.h"
#include "config.h"

ADS1220::ADS1220(uint8_t csPin) : _csPin(csPin) {}

void ADS1220::begin() {
  pinMode(_csPin, OUTPUT);
  digitalWrite(_csPin, HIGH);
  
  reset();
  
  // REG0: MUX=0000, GAIN=000, PGA_BYPASS=0
  writeRegister(0x00, 0x00);
  
  // REG1: DR=010 (20 SPS), MODE=00, CM=0, TS=0, BCS=0
  writeRegister(0x01, 0x40);
  
  // REG2: VREF=00 (Internal 2.048V)
  writeRegister(0x02, 0x00);
  
  // REG3: I1MUX=111, I2MUX=111 (Disabled)
  writeRegister(0x03, 0xFF);
}

void ADS1220::reset() {
  digitalWrite(_csPin, LOW);
  delayMicroseconds(10);
  SPI.transfer(ADS1220_CMD_RESET);
  digitalWrite(_csPin, HIGH);
  delay(100);
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
  SPI.transfer(ADS1220_CMD_RREG | (reg << 2));
  uint8_t value = SPI.transfer(0x00);
  digitalWrite(_csPin, HIGH);
  return value;
}

int32_t ADS1220::readChannel(uint8_t channel) {
  // Configurar MUX (single-ended vs AVSS)
  uint8_t mux_config = (channel << 4) | 0x0C;
  writeRegister(0x00, mux_config);
  
  delay(10);
  
  // Iniciar conversão
  digitalWrite(_csPin, LOW);
  delayMicroseconds(10);
  SPI.transfer(ADS1220_CMD_START);
  digitalWrite(_csPin, HIGH);
  
  delay(100);
  
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
  return (raw * 2.048) / 8388608.0;
}