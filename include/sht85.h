// sht85.h - Driver para sensor SHT85
#ifndef SHT85_H
#define SHT85_H

#include <Arduino.h>
#include <Wire.h>

// ============================================================================
// CLASSE SHT85
// ============================================================================
class SHT85 {
public:
  SHT85(uint8_t i2cAddress = 0x44);
  
  bool begin();
  bool readData(float &temperature, float &humidity);
  void reset();
  
private:
  uint8_t _address;
};

#endif // SHT85_H