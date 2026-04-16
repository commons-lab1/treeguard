# TreeGuard Firmware

## Variants

| Directory | MCU | Radio | Build system |
|-----------|-----|-------|-------------|
| `treeguard-mini/` | ATtiny1614 | None | PlatformIO (MegaTinyCore) |
| `treeguard-net/` | ESP32-C3 | BLE/WiFi | PlatformIO (ESP-IDF / Arduino) |

## Prerequisites

- [PlatformIO CLI](https://platformio.org/install/cli) or PlatformIO IDE extension
- USB-C cable (ESP32-C3) or UPDI programmer (ATtiny1614)

## Build & flash

```bash
# MVP (ATtiny)
cd treeguard-mini
pio run -t upload

# Connected (ESP32-C3)
cd treeguard-net
pio run -t upload
```

## Self-test mode

**MVP (ATtiny):** Jumper PB3 to GND before powering on. The device will:
1. Flash red LED 3 times
2. Flash green LED 3 times
3. Double-blink amber (both LEDs)
4. Read sensor and blink the raw value in binary on the red LED (long=1, short=0)
5. Show battery status (green=OK, red=low)
6. Enter normal operation

Remove the jumper after testing.

**V1 (ESP32-C3):** Self-test via serial monitor or BLE command (planned).

## Configuration

### MVP: DIP switch
Set species preset via the 3-bit DIP switch (3 switches, 6 presets). See [species presets](../docs/02-species-presets.md) for the switch-to-preset mapping table. Power cycle after changing.

### V1: BLE + QR
Scan the QR code on the frame to open the configuration page. Species, thresholds, and night schedule are configurable via BLE write characteristics.
