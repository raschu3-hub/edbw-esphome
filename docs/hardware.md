# Hardware notes

## Identified components

- ESP32-C3 module
- 8 MB SPI flash
- SP3232 RS232 level converter
- Native USB-C connection to ESP32-C3 USB-Serial/JTAG
- RJ45 connection to the EDECOA EM624A

## Confirmed UART mapping

```text
ESP32-C3 GPIO4 (TX) -> SP3232 -> EM624A RX
ESP32-C3 GPIO5 (RX) <- SP3232 <- EM624A TX
```

Serial parameters:

```text
2400 baud
8 data bits
no parity
1 stop bit
```

## PIP test frames

```text
QPI:
51 50 49 BE AC 0D

Expected response:
28 50 49 33 30 9A 0B 0D
(PI30 + CRC + CR

QPIRI:
51 50 49 52 49 F8 54 0D
```

The EM624A PC interface is RS232. The RS485 function on the combined connector may be intended for battery/BMS communication depending on model and firmware.
