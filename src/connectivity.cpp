// connectivity.cpp - Implementação WiFi e MQTT
#include "connectivity.h"
#include "config.h"

Connectivity::Connectivity() 
  : _mqttClient(_wifiClient),
    _wifiConnected(false),
    _mqttConnected(false),
    _lastReconnectAttempt(0) {}

bool Connectivity::begin() {
  connectWiFi();
  
  if (_wifiConnected) {
    connectMQTT();
  }
  
  return _wifiConnected && _mqttConnected;
}

void Connectivity::connectWiFi() {
  if (WiFi.status() == WL_CONNECTED) {
    _wifiConnected = true;
    return;
  }
  
  Serial.print(F("Conectando ao WiFi: "));
  Serial.println(WIFI_SSID);
  
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(F("."));
    attempts++;
  }
  Serial.println();
  
  if (WiFi.status() == WL_CONNECTED) {
    _wifiConnected = true;
    Serial.println(F("WiFi conectado!"));
    Serial.print(F("IP: "));
    Serial.println(WiFi.localIP());
    Serial.print(F("RSSI: "));
    Serial.print(WiFi.RSSI());
    Serial.println(F(" dBm"));
  } else {
    _wifiConnected = false;
    Serial.println(F("Falha ao conectar WiFi!"));
  }
}

void Connectivity::connectMQTT() {
  if (_mqttClient.connected()) {
    _mqttConnected = true;
    return;
  }
  
  if (!_wifiConnected) {
    connectWiFi();
    if (!_wifiConnected) return;
  }
  
  Serial.print(F("Conectando ao MQTT: "));
  Serial.println(MQTT_BROKER);
  
  _mqttClient.setId(MQTT_CLIENT_ID);
  
  if (strlen(MQTT_USER) > 0) {
    _mqttClient.setUsernamePassword(MQTT_USER, MQTT_PASS);
  }
  
  if (_mqttClient.connect(MQTT_BROKER, MQTT_PORT)) {
    _mqttConnected = true;
    Serial.println(F("MQTT conectado!"));
  } else {
    _mqttConnected = false;
    Serial.print(F("Falha MQTT! Código: "));
    Serial.println(_mqttClient.connectError());
  }
}

void Connectivity::checkConnections() {
  // Verificar WiFi
  if (WiFi.status() != WL_CONNECTED) {
    _wifiConnected = false;
    _mqttConnected = false;
    Serial.println(F("WiFi desconectado!"));
    connectWiFi();
  }
  
  // Verificar MQTT
  if (_wifiConnected && !_mqttClient.connected()) {
    _mqttConnected = false;
    unsigned long now = millis();
    if (now - _lastReconnectAttempt > RECONNECT_INTERVAL_MS) {
      _lastReconnectAttempt = now;
      Serial.println(F("MQTT desconectado!"));
      connectMQTT();
    }
  }
  
  // Manter MQTT vivo
  if (_mqttConnected) {
    _mqttClient.poll();
  }
}

void Connectivity::update() {
  checkConnections();
}

bool Connectivity::sendData(const SensorData &data) {
  if (!_mqttConnected) {
    Serial.println(F("MQTT não conectado. Dados não enviados."));
    return false;
  }
  
  // Construir JSON
  String json = "{";
  json += "\"ts\":" + String(data.timestamp) + ",";
  json += "\"FGT11\":" + String(data.windSpeed, 2) + ",";
  json += "\"LT11\":" + String(data.waterLevel, 3) + ",";
  json += "\"TT11\":" + String(data.temp1, 2) + ",";
  json += "\"TT12\":" + String(data.temp2, 2) + ",";
  json += "\"TT13\":" + String(data.temp3, 2) + ",";
  json += "\"THT11_T\":" + String(data.airTemp, 2) + ",";
  json += "\"THT11_H\":" + String(data.humidity, 1) + ",";
  json += "\"RT11\":" + String(data.radiation, 1);
  json += "}";
  
  // Enviar via MQTT
  _mqttClient.beginMessage(MQTT_TOPIC);
  _mqttClient.print(json);
  _mqttClient.endMessage();
  
  Serial.print(F("✓ MQTT: "));
  Serial.println(json);
  
  return true;
}