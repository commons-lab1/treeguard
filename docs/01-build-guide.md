# Build Guide

## Overview

TreeGuard has three build tiers. Start with the MVP.

| Tier | MCU | Radio | Power | Cost | Best for |
|------|-----|-------|-------|------|----------|
| **MVP** | ATtiny1614 | None | CR123A battery (~1 year) | ~$5-8 | First deployments, mass scale |
| **V1** | ESP32-C3 | BLE | Solar + 18650 | ~$11-14 | Connected deployments |
| **V2** | ESP32-C3 | BLE + LoRa | Solar + 18650 | ~$20-25 | Municipal networks |

## Tools needed

- Soldering iron + solder
- Wire strippers
- 3D printer (or access to one) with PETG filament
- Multimeter
- Allen key set + T10 security Torx driver
- Hot glue gun or RTV silicone

## MVP build (ATtiny1614)

### Step 1: Electronics assembly

**Components:**
- ATtiny1614 (SOIC-14 on breakout board, or bare with UPDI programmer)
- Capacitive soil moisture sensor v2.0
- 2x 5mm diffused LEDs (1 red, 1 green)
- 2x 220 ohm resistors
- CR123A battery + holder
- Stripboard or custom PCB

**Wiring:**
```
ATtiny1614
├── PA1 ──── Soil sensor analog out
├── PA2 ──── DIP switch 1 (to GND when ON)
├── PA3 ──── DIP switch 2 (to GND when ON)
├── PA4 ──── 220R ──── Red LED ──── GND
├── PA5 ──── 220R ──── Green LED ── GND
├── PA6 ──── Sensor VCC (switched power)
├── PA7 ──── DIP switch 3 (to GND when ON)
├── PB3 ──── Self-test jumper (to GND = run self-test on boot)
├── VCC ──── Battery +
└── GND ──── Battery - / Sensor GND / LED cathodes / DIP common
```

> **Power saving:** The sensor is powered via PA6 so firmware can cut power between readings. This is critical for CR123A battery life.

> **Self-test:** Jumper PB3 to GND before powering on to run the LED + sensor self-test sequence. Remove jumper for normal operation.

### Step 2: Conformal coating

Apply 2 coats of conformal coating (MG Chemicals 422B or equivalent) to the assembled PCB. This is non-optional for outdoor use. Let each coat dry 30 minutes.

### Step 3: Enclosure

Export `electronics-enclosure.stl` from the OpenSCAD file (`openscad -o enclosure.stl -D 'part="enclosure"' treeguard-frame.scad`) and print in PETG. Apply silicone bead in the gasket groove. Press-fit the PCB. Route sensor wire and LED wires through the cable gland holes.

### Step 4: Frame assembly

1. Print 3x hex ring segments + 3x vertical spines
2. Press M3 brass inserts into spine mounting holes (soldering iron tip, 220C)
3. Stack rings onto spines, secure with M3 security Torx screws
4. Clip electronics enclosure onto south-facing spine
5. Route sensor probe down inside one spine to ground level

### Step 5: Deployment

See [03-deployment-checklist.md](03-deployment-checklist.md).

---

## V1 build (ESP32-C3)

*Detailed guide coming after MVP field validation.*

Core differences from MVP:
- ESP32-C3 Super Mini replaces ATtiny
- Add TP4056 charge module + 18650 battery
- Add 5V solar panel on tilt mount
- Firmware includes BLE advertising for PWA

## Alternative frames (no 3D printer)

### PVC pipe version
- 3x 32mm PVC pipes, 70cm length
- PP mesh or chicken wire zip-tied between pipes
- 3x ground stakes (rebar or thick wire)
- Electronics enclosure: weatherproof junction box (~$2)
- Total frame cost: ~$3-5

### Laser-cut plywood version
- See `mechanical/frame-plywood/` for DXF files
- 6mm marine plywood recommended
- Coat with outdoor wood sealer
- Slots together, no glue needed

### Repurposed crate version
- Flip a plastic storage crate upside down
- Cut ventilation holes with a hole saw
- Mount electronics box with cable ties
- Fastest and cheapest option for urban pilots
