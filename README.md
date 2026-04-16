# TreeGuard

**Open-source young tree protection frame with LED alert + soil moisture sensor.**

A cheap (~$12), volunteer-built device that wraps around a sapling, monitors soil moisture, and blinks red when the tree is thirsty. Passersby water it with a 1-1.5 L bottle. Green means happy. That's it.

---

## Why this exists

Millions of saplings die in their first two summers because nobody waters them consistently. Irrigation is expensive. Volunteers forget. TreeGuard turns every passerby into a potential caretaker with a single visual signal.

## The 5 risks we solve (or we fail)

| # | Risk | Our answer |
|---|------|------------|
| 1 | **Overwatering** — 10 people each dump water | 18-hour post-water lockout; green LED = "already watered, move on" |
| 2 | **Alert fatigue** — blinking LED becomes wallpaper | Smart thresholds; LED is OFF by default, only activates when truly needed |
| 3 | **Vandalism / theft** | Nothing worth stealing; all parts <$3 replaceable; security Torx fasteners |
| 4 | **Plastic to save trees** | Recycled PETG; frame is reusable across saplings; documented lifecycle |
| 5 | **Maintenance orphaning** | Self-reporting health (amber = help me); adopt-a-tree model; auto-sleep on dead battery |

## Three LED states

| State | LED | Meaning | Duration |
|-------|-----|---------|----------|
| Thirsty | Red slow pulse | Soil is dry — please water ~1 L | Until watered |
| Happy | Green breath | Just watered — no more needed | ~2 hours, then OFF |
| Sick | Amber double-blink | Battery low or sensor fault | Until maintained |

LEDs are suppressed between 21:00 and sunrise (wildlife / light pollution).

## Hardware at a glance

- **MCU:** ESP32-C3 Super Mini (connected) or ATtiny1614 (offline MVP)
- **Sensor:** Capacitive soil moisture v2.0 (NOT resistive — corrodes in weeks)
- **Power:** 5V 0.5-1W solar panel + TP4056 + reused 18650 Li-ion
- **Frame:** PETG 3D-printed hex segments (or PVC pipe / plywood alternatives)
- **Target BOM:** ~$11-14

See [hardware/bom.csv](hardware/bom.csv) for full bill of materials.

## Quick start

1. Read [docs/00-before-you-plant.md](docs/00-before-you-plant.md) first
2. Source parts from [hardware/bom.csv](hardware/bom.csv)
3. Follow [docs/01-build-guide.md](docs/01-build-guide.md)
4. Flash firmware per [firmware/README.md](firmware/README.md)
5. Deploy per [docs/03-deployment-checklist.md](docs/03-deployment-checklist.md)

## Project structure

```
treeguard/
├── docs/                 Build guides, species presets, rationale
├── hardware/             KiCad schematics, PCB, BOM
├── mechanical/           3D print files, PVC/plywood alternatives, signage
├── firmware/
│   ├── treeguard-mini/   ATtiny offline MVP
│   └── treeguard-net/    ESP32-C3 connected version
├── app/pwa/              Progressive web app (stretch goal)
└── data/field-logs/      Anonymized deployment data
```

## Licenses

- **Hardware:** [CERN-OHL-S v2](LICENSE-HARDWARE)
- **Software:** [MIT](LICENSE-SOFTWARE)
- **Docs & Media:** [CC BY-SA 4.0](LICENSE-DOCS-MEDIA)

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md). We use DCO sign-off.

## Status

**Pre-prototype.** Design analysis complete. Building first 3 units for field testing.
