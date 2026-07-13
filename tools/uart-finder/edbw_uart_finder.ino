/*
  EDECOA ED-BW UART Finder
  Target: ESP32-C3 based ED-BW_COMM V3.1
  Purpose: Find the UART pins connected to the onboard SP3232 RS232 transceiver.

  IMPORTANT:
  - Connect the ED-BW to the powered EM624A during the scan.
  - USB CDC must be enabled.
  - Serial Monitor: 115200 baud.
*/

#define ARDUINO_USB_CDC_ON_BOOT 1

#include <Arduino.h>
#include <HardwareSerial.h>

HardwareSerial inverterSerial(1);

const uint8_t candidatePins[] = {
  0, 1, 2, 3, 4, 5, 6, 7, 10, 20, 21
};

constexpr size_t candidateCount =
    sizeof(candidatePins) / sizeof(candidatePins[0]);

const uint8_t qpiCommand[] = {
  0x51, 0x50, 0x49, 0xBE, 0xAC, 0x0D
};

const uint8_t qpiriCommand[] = {
  0x51, 0x50, 0x49, 0x52, 0x49, 0xF8, 0x54, 0x0D
};

constexpr uint32_t UART_BAUD = 2400;
constexpr uint32_t QPI_TIMEOUT_MS = 700;
constexpr uint32_t QPIRI_TIMEOUT_MS = 1500;

uint8_t responseBuffer[256];

uint16_t pipCrc(const uint8_t *data, size_t length) {
  uint16_t crc = 0;

  for (size_t i = 0; i < length; i++) {
    crc ^= static_cast<uint16_t>(data[i]) << 8;

    for (uint8_t bit = 0; bit < 8; bit++) {
      if (crc & 0x8000) {
        crc = static_cast<uint16_t>((crc << 1) ^ 0x1021);
      } else {
        crc <<= 1;
      }
    }
  }
  return crc;
}

uint8_t escapeCrcByte(uint8_t value) {
  if (value == 0x28 || value == 0x0D || value == 0x0A) {
    return value + 1;
  }
  return value;
}

void printHex(const uint8_t *data, size_t length) {
  Serial.print("HEX   : ");
  for (size_t i = 0; i < length; i++) {
    if (data[i] < 0x10) Serial.print('0');
    Serial.print(data[i], HEX);
    Serial.print(' ');
  }
  Serial.println();
}

void printAscii(const uint8_t *data, size_t length) {
  Serial.print("ASCII : ");
  for (size_t i = 0; i < length; i++) {
    const uint8_t value = data[i];

    if (value >= 32 && value <= 126) {
      Serial.write(value);
    } else if (value == '\r') {
      Serial.print("<CR>");
    } else {
      Serial.print('.');
    }
  }
  Serial.println();
}

void clearUart() {
  while (inverterSerial.available() > 0) {
    inverterSerial.read();
  }
}

size_t readResponse(uint32_t timeoutMs) {
  size_t length = 0;
  const uint32_t start = millis();
  uint32_t lastByteTime = start;

  while (millis() - start < timeoutMs) {
    while (inverterSerial.available() > 0) {
      const int value = inverterSerial.read();
      if (value < 0) continue;

      if (length < sizeof(responseBuffer)) {
        responseBuffer[length++] = static_cast<uint8_t>(value);
      }

      lastByteTime = millis();

      if (value == 0x0D) {
        return length;
      }
    }

    if (length > 0 && millis() - lastByteTime > 150) {
      break;
    }

    delay(1);
  }

  return length;
}

size_t sendCommand(const uint8_t *command,
                   size_t commandLength,
                   uint32_t timeoutMs) {
  clearUart();
  inverterSerial.write(command, commandLength);
  inverterSerial.flush();
  return readResponse(timeoutMs);
}

bool isValidPipResponse(const uint8_t *data, size_t length) {
  if (length < 6 || data[0] != '(' || data[length - 1] != 0x0D) {
    return false;
  }

  const size_t payloadLength = length - 3;
  const uint16_t calculated = pipCrc(data, payloadLength);

  const uint8_t expectedHigh =
      escapeCrcByte(static_cast<uint8_t>(calculated >> 8));
  const uint8_t expectedLow =
      escapeCrcByte(static_cast<uint8_t>(calculated & 0xFF));

  const uint8_t receivedHigh = data[length - 3];
  const uint8_t receivedLow = data[length - 2];

  if (expectedHigh == receivedHigh && expectedLow == receivedLow) {
    return true;
  }

  // Strong fallback for the short QPI response "(PI30..."
  return length >= 5 &&
         data[0] == '(' &&
         data[1] == 'P' &&
         data[2] == 'I';
}

bool testPair(uint8_t txPin, uint8_t rxPin,
              size_t testNumber, size_t totalTests) {
  Serial.println();
  Serial.println("================================================");
  Serial.printf("Test %u/%u: TX=GPIO%u, RX=GPIO%u\n",
                static_cast<unsigned>(testNumber),
                static_cast<unsigned>(totalTests),
                txPin, rxPin);

  inverterSerial.end();
  delay(50);
  inverterSerial.begin(UART_BAUD, SERIAL_8N1, rxPin, txPin);
  delay(100);

  const size_t qpiLength =
      sendCommand(qpiCommand, sizeof(qpiCommand), QPI_TIMEOUT_MS);

  if (qpiLength == 0) {
    Serial.println("No response.");
    return false;
  }

  printHex(responseBuffer, qpiLength);
  printAscii(responseBuffer, qpiLength);

  if (!isValidPipResponse(responseBuffer, qpiLength)) {
    Serial.println("Data received, but not a valid PIP response.");
    return false;
  }

  Serial.println("QPI recognized. Confirming with QPIRI...");

  const size_t qpiriLength =
      sendCommand(qpiriCommand, sizeof(qpiriCommand), QPIRI_TIMEOUT_MS);

  if (qpiriLength == 0) {
    Serial.println("QPI matched, but QPIRI did not confirm.");
    return false;
  }

  printHex(responseBuffer, qpiriLength);
  printAscii(responseBuffer, qpiriLength);

  if (!isValidPipResponse(responseBuffer, qpiriLength)) {
    Serial.println("QPIRI response was not valid.");
    return false;
  }

  Serial.println();
  Serial.println("################################################");
  Serial.println("UART FOUND");
  Serial.printf("TX: GPIO%u\n", txPin);
  Serial.printf("RX: GPIO%u\n", rxPin);
  Serial.println("Baud: 2400, 8N1");
  Serial.println("################################################");

  return true;
}

void setup() {
  Serial.begin(115200);
  delay(2500);

  Serial.println();
  Serial.println("================================================");
  Serial.println("EDECOA ED-BW UART Finder");
  Serial.println("Connect the EM624A to the ED-BW RJ45 port.");
  Serial.println("================================================");
}

void loop() {
  static bool finished = false;

  if (finished) {
    delay(1000);
    return;
  }

  finished = true;

  const size_t totalTests = candidateCount * (candidateCount - 1);
  size_t testNumber = 0;

  for (size_t txIndex = 0; txIndex < candidateCount; txIndex++) {
    for (size_t rxIndex = 0; rxIndex < candidateCount; rxIndex++) {
      if (txIndex == rxIndex) continue;

      testNumber++;

      if (testPair(candidatePins[txIndex],
                   candidatePins[rxIndex],
                   testNumber,
                   totalTests)) {
        while (true) {
          delay(1000);
        }
      }
    }
  }

  Serial.println();
  Serial.println("Scan completed: no valid UART pair found.");
}
