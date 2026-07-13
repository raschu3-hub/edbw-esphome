# Troubleshooting

## Device is offline when not connected to the inverter

The ED-BW must still be powered through USB-C. A missing inverter only causes PIP polling timeouts; it should not prevent Wi-Fi from working.

## PIP timeouts

Check:

```yaml
tx_pin: GPIO4
rx_pin: GPIO5
baud_rate: 2400
```

The dongle must be connected to the powered EM624A through its original RJ45 connection.

## Wi-Fi works but values are unavailable

Look for both directions in the logs:

```text
>>> transmitted command
<<< inverter response
```

If only `>>>` appears, the inverter response is not reaching GPIO5.

## USB serial output is blank

In Arduino IDE enable:

```text
USB CDC On Boot: Enabled
```

Then reopen the serial monitor after uploading.

## ESPHome secret not found

Add to the ESPHome `secrets.yaml`:

```yaml
fallback_password: "CHANGE_ME_123"
```
