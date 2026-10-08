# ESP32 Telemetry Node

A compact IoT project that reads temperature and humidity from a DHT22 sensor and sends telemetry from an ESP32 to a REST API over Wi-Fi.

## Features

- ESP32 firmware with PlatformIO
- DHT22 temperature and humidity sampling
- JSON telemetry over HTTP
- Non-blocking Wi-Fi retry logic
- Serial diagnostics
- Secrets kept out of Git
- GitHub Actions firmware build

## Hardware

- ESP32 development board
- DHT22 sensor
- 10 kΩ pull-up resistor
- USB cable

## Wiring

| DHT22 | ESP32 |
|---|---|
| VCC | 3.3V |
| DATA | GPIO 4 |
| GND | GND |

## Quick start

1. Install PlatformIO.
2. Copy `include/secrets.example.h` to `include/secrets.h`.
3. Enter your Wi-Fi credentials and API endpoint.
4. Connect the ESP32 over USB.
5. Build and upload:

```bash
pio run
pio run --target upload
pio device monitor
```

## Example telemetry

```json
{
  "device_id": "esp32-lab-01",
  "temperature_c": 24.6,
  "humidity_percent": 51.2,
  "uptime_seconds": 183
}
```

## Repository layout

```text
.
├── .github/workflows/ci.yml
├── docs/architecture.md
├── include/secrets.example.h
├── src/main.cpp
├── .gitignore
├── platformio.ini
└── README.md
```

## What I practiced

- Embedded C++ on ESP32
- Sensor integration
- Wi-Fi networking
- REST API communication
- Basic reliability and retry handling
- CI builds for embedded firmware

## Roadmap

- MQTT transport
- Offline buffering
- FastAPI backend
- SQLite/PostgreSQL storage
- Web dashboard
- OTA firmware updates
