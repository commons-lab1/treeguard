# Species Moisture Presets

TreeGuard ships with moisture threshold presets for common sapling species. Set the correct preset before deployment.

## How thresholds work

The capacitive sensor returns an analog value (0-4095 on ESP32, 0-1023 on ATtiny). After calibration:
- **0%** = completely dry (sensor in air)
- **100%** = saturated (sensor in water)

Each preset defines:
- `T_dry`: below this = "thirsty" (red LED triggers after 6h sustained)
- `T_wet`: above this after a jump = "just watered" (green LED for 2h)
- `lockout_hours`: minimum hours between thirsty alerts after watering

## Preset table

| ID | Category | Example species | T_dry (%) | T_wet (%) | Lockout (h) | Notes |
|----|----------|-----------------|-----------|-----------|-------------|-------|
| 0 | Default / Unknown | Any | 30 | 50 | 18 | Conservative starting point |
| 1 | High water | Willow, Birch, Poplar | 40 | 55 | 12 | These species drink heavily |
| 2 | Moderate water | Oak, Maple, Linden, Elm | 30 | 50 | 18 | Most common urban saplings |
| 3 | Low water | Pine, Olive, Juniper | 20 | 40 | 24 | Drought-tolerant; overwatering risk higher |
| 4 | Fruit trees | Apple, Cherry, Pear | 35 | 55 | 16 | Need consistent moisture for establishment |
| 5 | Mediterranean | Cypress, Cork Oak, Carob | 15 | 35 | 24 | Adapted to dry; very easy to overwater |

## Setting the preset

### MVP (ATtiny version)
Use the 3-position DIP switch on the PCB:
- Switch pattern maps to preset ID (see silkscreen label)
- Power cycle after changing

### V1 (ESP32 version)
- Scan the QR code on the frame
- Select species on the configuration page
- Threshold updates via BLE

## Calibration

These presets are starting points. Every soil type and microclimate is different.

**Field calibration procedure:**
1. Install sensor in dry soil (hasn't rained for 3+ days). Note the reading = your local "dry".
2. Water thoroughly (2L). Wait 30 minutes for percolation. Note the reading = your local "wet".
3. Adjust T_dry and T_wet if they differ significantly from the preset.

The adaptive firmware (V1+) does this automatically using a 7-day rolling min/max baseline.

## Contributing presets

Know a species we don't cover? Open a PR adding it to this table and to `firmware/treeguard-*/src/presets.h`. Include:
- Species name (common + Latin)
- Your region/climate zone
- How you determined the thresholds (field data preferred)
