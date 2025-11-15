// sht85.cpp - Implementação do driver SHT85
#include "sht85.h"

SHT85::SHT85(uint8_t i2cAddress) : _address(i2cAddress) {}

bool SHT85::begin() {
  Wire.begin();
  Wire.setClock(100000); // 100 kHz I2C
  
  reset();
  
  delay(50);
  return true;
}

void SHT85::reset() {
  Wire.beginTransmission(_address);
  Wire.write(0x30);
  Wire.write(0xA2);
  Wire.endTransmission();
  delay(50);
}

bool SHT85::readData(float &temperature, float &humidity) {
  // Enviar comando de medição (high repeatability)
  Wire.beginTransmission(_address);
  Wire.write(0x2C);
  Wire.write(0x06);
  if (Wire.endTransmission() != 0) {
    return false;
  }
  
  delay(20);
  
  // Ler 6 bytes
  Wire.requestFrom(_address, (uint8_t)6);
  
  if (Wire.available() < 6) {
    return false;
  }
  
  uint8_t data[6];
  for (int i = 0; i < 6; i++) {
    data[i] = Wire.read();
  }
  
  // Converter temperatura
  uint16_t rawTemp = (data[0] << 8) | data[1];
  temperature = -45.0 + 175.0 * (rawTemp / 65535.0);
  
  // Converter umidade
  uint16_t rawHum = (data[3] << 8) | data[4];
  humidity = 100.0 * (rawHum / 65535.0);
  
  return true;
}