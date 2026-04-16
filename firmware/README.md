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

Hold the reset button for 3 seconds on power-up. The device will:
1. Cycle red LED (1s)
2. Cycle green LED (1s)
3. Cycle amber (both LEDs, 1s)
4. Read sensor and blink the raw value in binary on the red LED
5. Enter normal operation

## Configuration

### MVP: DIP switch
Set species preset via the 3-position DIP switch. See [species presets](../docs/02-species-presets.md).

### V1: BLE + QR
Scan the QR code on the frame to open the configuration page. Species, thresholds, and night schedule are configurable.
