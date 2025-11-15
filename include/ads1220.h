// ads1220.h - Driver para ADC ADS1220
#ifndef ADS1220_H
#define ADS1220_H

#include <Arduino.h>
#include <SPI.h>

// ============================================================================
// CLASSE ADS1220
// ============================================================================
class ADS1220 {
public:
  ADS1220(uint8_t csPin);
  
  void begin();
  void reset();
  int32_t readChannel(uint8_t channel);
  float toVoltage(int32_t raw);
  
private:
  uint8_t _csPin;
  
  void writeRegister(uint8_t reg, uint8_t value);
  uint8_t readRegister(uint8_t reg);
};

#endif // ADS1220_H