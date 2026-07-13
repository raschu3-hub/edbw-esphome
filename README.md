<p align="center">
  <img src="docs/images/banner.png" width="900">
</p>

<h1 align="center">
EDECOA ED-BW_COMM V3.1 ESPHome Firmware
</h1>

<p align="center">

Replace the original cloud firmware of the **EDECOA ED-BW_COMM V3.1 (ESP32-C3)** with **ESPHome** and integrate your **EM624A inverter** directly into **Home Assistant**.

</p>

<p align="center">

![ESPHome](https://img.shields.io/badge/ESPHome-Compatible-blue)
![Home Assistant](https://img.shields.io/badge/Home%20Assistant-Supported-41BDF5)
![ESP32-C3](https://img.shields.io/badge/ESP32-C3-green)
![License](https://img.shields.io/badge/License-MIT-orange)

</p>

---

# Features

- ESPHome firmware for the original ED-BW_COMM V3.1
- Local communication (no cloud required)
- Native Home Assistant integration
- OTA firmware updates
- Real-time inverter values
- Voltronic / PIP protocol support
- UART debugging

---

# Tested Hardware

| Device | Status |
|---------|:------:|
| ED-BW_COMM V3.1 | ✅ |
| ESP32-C3 | ✅ |
| EDECOA EM624A 6.2 kW / 48 V | ✅ |
| ESPHome | ✅ |
| Home Assistant | ✅ |

---

# Hardware

<p align="center">
<img src="docs/images/edbw-front.jpg" width="650">
</p>

Original EDECOA ED-BW_COMM V3.1 WiFi module.

<p align="center">
<img src="docs/images/edbw-back.jpg" width="650">
</p>

Rear side of the PCB.

---

# UART Configuration

Confirmed working configuration:

| Parameter | Value |
|-----------|-------|
| TX | GPIO4 |
| RX | GPIO5 |
| Baud Rate | 2400 |
| Data Bits | 8 |
| Parity | None |
| Stop Bits | 1 |

No hardware modifications are required.

The original ED-BW module can be flashed directly using its USB interface.

---

# System Architecture

```text
                Home Assistant
                       ▲
                       │ API
                       │
                 ESPHome Firmware
                       ▲
                       │
             ED-BW_COMM V3.1 (ESP32-C3)
                       │
                 RS232 / PIP Protocol
                       │
                 EDECOA EM624A
```

---

# ESPHome Web Interface

<p align="center">
<img src="docs/images/web-overview.png" width="100%">
</p>

Live monitoring of the inverter directly from the ESPHome web interface.

Features include:

- Live sensor values
- UART debug log
- OTA updates
- Web server diagnostics

---

# Energy Control Center Example

The firmware is also used as part of a custom ESPHome based Energy Control Center.

<p align="center">
<img src="docs/images/LCD_dashboard.png" width="70%">
</p>

Integrated systems include:

- EDECOA EM624A
- JK-BMS
- Victron SmartSolar
- Hoymiles HMS-2000
- Zendure Hub
- go-e Charger

---

# Installation

## 1. Backup the original firmware

```bash
python -m esptool -p COM7 read-flash 0x000000 0x800000 backup.bin
```

---

## 2. Copy

```
esphome/edecoa-edbw.yaml
```

into your ESPHome configuration directory.

---

## 3. Configure secrets.yaml

```yaml
wifi_ssid: "YOUR_WIFI"
wifi_password: "YOUR_PASSWORD"
fallback_password: "CHANGE_ME"
```

---

## 4. Flash via USB

The initial installation must be performed over USB.

Subsequent firmware updates can be installed over Wi-Fi using OTA.

---

# Repository Structure

```
docs/
    images/

esphome/

home-assistant/

tools/
    uart-finder/
```

---

# Troubleshooting

If no inverter data is received:

- Verify TX = GPIO4
- Verify RX = GPIO5
- Confirm 2400 baud
- Disable serial logger (`baud_rate: 0`)
- Check the RJ45 cable wiring

---

# Disclaimer

This project is not affiliated with EDECOA.

Flashing custom firmware replaces the original firmware.

Always create a firmware backup before flashing.

Use at your own risk.

---

# Contributing

Contributions, bug reports and pull requests are welcome.

If you test another inverter model, please include:

- inverter model
- ESPHome version
- board revision
- UART logs

---

# License

MIT License