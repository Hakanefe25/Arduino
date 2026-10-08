# Architecture

```text
DHT22 sensor
    |
    v
ESP32 firmware
    |
    | JSON over HTTP
    v
REST API endpoint
    |
    +--> database
    +--> dashboard / alerts
```

The firmware samples the DHT22 every 15 seconds, creates a compact JSON payload and sends it to a configurable REST endpoint.

## Reliability choices

- Wi-Fi connection attempts are rate-limited instead of blocking forever.
- Sensor read failures are logged and skipped.
- HTTP requests have explicit timeouts.
- Credentials and private endpoints live in `include/secrets.h`, which is ignored by Git.

## Example payload

```json
{
  "device_id": "esp32-lab-01",
  "temperature_c": 24.6,
  "humidity_percent": 51.2,
  "uptime_seconds": 183
}
```
