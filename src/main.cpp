#include <Arduino.h>
#include <DHT.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include "secrets.h"

namespace {
constexpr uint8_t DHT_PIN = 4;
constexpr uint8_t DHT_TYPE = DHT22;
constexpr unsigned long SAMPLE_INTERVAL_MS = 15000;
constexpr unsigned long WIFI_RETRY_INTERVAL_MS = 5000;

DHT dht(DHT_PIN, DHT_TYPE);
unsigned long lastSampleAt = 0;
unsigned long lastWifiAttemptAt = 0;

void connectWifi() {
  if (WiFi.status() == WL_CONNECTED) {
    return;
  }

  const unsigned long now = millis();
  if (now - lastWifiAttemptAt < WIFI_RETRY_INTERVAL_MS) {
    return;
  }

  lastWifiAttemptAt = now;
  Serial.printf("[wifi] connecting to %s\n", WIFI_SSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

bool readSensor(float &temperatureC, float &humidityPercent) {
  humidityPercent = dht.readHumidity();
  temperatureC = dht.readTemperature();

  if (isnan(temperatureC) || isnan(humidityPercent)) {
    Serial.println("[sensor] failed to read DHT22");
    return false;
  }

  return true;
}

String buildPayload(float temperatureC, float humidityPercent) {
  String payload = "{";
  payload += "\"device_id\":\"" + String(DEVICE_ID) + "\",";
  payload += "\"temperature_c\":" + String(temperatureC, 1) + ",";
  payload += "\"humidity_percent\":" + String(humidityPercent, 1) + ",";
  payload += "\"uptime_seconds\":" + String(millis() / 1000UL);
  payload += "}";
  return payload;
}

void sendTelemetry(const String &payload) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("[http] skipped: Wi-Fi is offline");
    return;
  }

  HTTPClient http;
  http.setConnectTimeout(5000);
  http.setTimeout(5000);
  http.begin(API_ENDPOINT);
  http.addHeader("Content-Type", "application/json");

  const int statusCode = http.POST(payload);
  if (statusCode > 0) {
    Serial.printf("[http] POST %s -> %d\n", API_ENDPOINT, statusCode);
  } else {
    Serial.printf("[http] request failed: %s\n", http.errorToString(statusCode).c_str());
  }

  http.end();
}
}  // namespace

void setup() {
  Serial.begin(115200);
  delay(400);

  Serial.println("\nESP32 Telemetry Node");
  dht.begin();
  connectWifi();
}

void loop() {
  connectWifi();

  if (WiFi.status() == WL_CONNECTED) {
    static bool announced = false;
    if (!announced) {
      Serial.printf("[wifi] connected, IP: %s\n", WiFi.localIP().toString().c_str());
      announced = true;
    }
  }

  const unsigned long now = millis();
  if (now - lastSampleAt < SAMPLE_INTERVAL_MS) {
    delay(20);
    return;
  }
  lastSampleAt = now;

  float temperatureC = 0.0F;
  float humidityPercent = 0.0F;
  if (!readSensor(temperatureC, humidityPercent)) {
    return;
  }

  const String payload = buildPayload(temperatureC, humidityPercent);
  Serial.printf("[telemetry] %s\n", payload.c_str());
  sendTelemetry(payload);
}
