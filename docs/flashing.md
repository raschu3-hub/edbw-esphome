# Flashing guide

## Arduino IDE settings for the UART finder

- Board: ESP32C3 Dev Module
- USB CDC On Boot: Enabled
- CPU Frequency: 160 MHz (WiFi)
- Flash Frequency: 80 MHz
- Flash Size: 8 MB
- Upload Speed: 115200
- Port: the COM port shown for the ED-BW

Serial monitor:

```text
115200 baud
```

## ESPHome

The provided YAML uses:

```yaml
esp32:
  board: esp32-c3-devkitm-1
  framework:
    type: esp-idf
```

The first installation should be done through USB. Future updates can use OTA.
