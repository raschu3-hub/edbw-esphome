# Backup and restore

## Read flash identification

```cmd
python -m esptool -p COM7 flash-id
```

Confirmed reference device:

```text
ESP32-C3
8 MB flash
DIO
80 MHz
```

## Create a full backup

```cmd
python -m esptool -p COM7 read-flash 0x000000 0x800000 edbw_backup.bin
```

Keep the file private. It may contain credentials, cloud endpoints, device identifiers and copyrighted vendor firmware.

## Restore

```cmd
python -m esptool -p COM7 erase-flash
python -m esptool -p COM7 write-flash 0x000000 edbw_backup.bin
```

Do not restore a firmware image from another device.
