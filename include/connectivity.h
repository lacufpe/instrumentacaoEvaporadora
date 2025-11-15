// connectivity.h - Gerenciamento de WiFi e MQTT
#ifndef CONNECTIVITY_H
#define CONNECTIVITY_H

#include <Arduino.h>
#include <WiFiS3.h>
#include <ArduinoMqttClient.h>
#include "sensor_data.h"

// ============================================================================
// CLASSE DE CONECTIVIDADE
// ============================================================================
class Connectivity {
public:
  Connectivity();
  
  bool begin();
  void update();
  bool sendData(const SensorData &data);
  
  bool isWiFiConnected() const { return _wifiConnected; }
  bool isMqttConnected() const { return _mqttConnected; }
  
private:
  WiFiClient _wifiClient;
  MqttClient _mqttClient;
  
  bool _wifiConnected;
  bool _mqttConnected;
  unsigned long _lastReconnectAttempt;
  
  void connectWiFi();
  void connectMQTT();
  void checkConnections();
};

#endif // CONNECTIVITY_H