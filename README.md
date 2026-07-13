<p align="center">
  <img src="docs/images/banner.png" width="900">
</p>

<h1 align="center">
EDECOA ED-BW_COMM V3.1 ESPHome Firmware
</h1>

<p align="center">

ESPHome firmware for the original **EDECOA ED-BW_COMM V3.1 (ESP32-C3)** WiFi module used with the **EDECOA EM624A** hybrid inverter.

Run your inverter completely **locally** without vendor cloud and integrate it directly into **Home Assistant**.

</p>

<p align="center">

![ESPHome](https://img.shields.io/badge/ESPHome-Compatible-blue)
![Home Assistant](https://img.shields.io/badge/Home%20Assistant-Supported-41BDF5)
![ESP32-C3](https://img.shields.io/badge/ESP32-C3-green)
![License](https://img.shields.io/badge/License-MIT-orange)

</p>

---

# Why?

The original ED-BW WiFi module communicates with the inverter using the
Voltronic / PIP protocol over RS232.

This project replaces the original firmware with **ESPHome**, allowing direct
integration into Home Assistant without cloud services.

✔ Local communication

✔ OTA updates

✔ ESPHome

✔ Home Assistant

✔ Real-time inverter data

---

# Tested Hardware

| Device | Status |
|---------|:------:|
| EDECOA ED-BW_COMM V3.1 | ✅ |
| ESP32-C3 | ✅ |
| EDECOA EM624A 6.2 kW / 48 V | ✅ |
| Home Assistant | ✅ |
| ESPHome | ✅ |

---

# Live Web Interface

<p align="center">
<img src="docs/images/esphome-webserver.png" width="100%">
</p>

The original ED-BW module running ESPHome.

Features shown:

- Live inverter values
- UART Debug
- PIP communication
- OTA support

---

# Hardware

<p align="center">
<img src="docs/images/edbw-front.jpg" width="700">
</p>

Original EDECOA ED-BW_COMM V3.1 hardware.

Confirmed UART:

| Signal | GPIO |
|--------|------|
| TX | GPIO4 |
| RX | GPIO5 |

Serial settings

```
2400 Baud
8 Data Bits
No Parity
1 Stop Bit
```

---

# Architecture

```
                 Home Assistant
                        ▲
                        │ API
                        │
                  ESPHome Firmware
                        ▲
                        │
                  ESP32-C3 (ED-BW)
                        │
                    RS232 (PIP)
                        │
                 EDECOA EM624A
```

---

# Installation

1. Backup the original firmware

```
python -m esptool -p COM7 read-flash 0x000000 0x800000 backup.bin
```

2. Copy

```
esphome/edecoa-edbw.yaml
```

to your ESPHome configuration.

3. Add

```yaml
wifi_ssid:
wifi_password:
fallback_password:
```

to your `secrets.yaml`.

4. Flash using USB.

5. Future updates can be done OTA.

---

# Home Assistant

<p align="center">
<img src="docs/images/homeassistant.png" width="100%">
</p>

Example entities:

- Battery Voltage
- Battery Capacity
- PV Voltage
- PV Charging Power
- AC Output Voltage
- AC Output Power
- Inverter Temperature
- Device Mode

---

# Real World Example

<p align="center">
<img src="docs/images/ecc-display.jpg" width="500">
</p>

This firmware is used inside a custom **Energy Control Center (ECC)** integrating:

- EDECOA EM624A
- JK-BMS
- Victron
- Hoymiles
- Zendure
- go-e Charger

---

# Repository Structure

```
docs/
esphome/
home-assistant/
tools/
```

---

# License

MIT License

---

# Acknowledgements

This project was developed by reverse engineering the original ED-BW_COMM V3.1 communication module and validating it on real EDECOA EM624A hardware.

Contributions, pull requests and testing on other inverter models are welcome.