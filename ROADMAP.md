# Roadmap

## MVP (current)

Target: 20 units deployed, 1 summer of field data.

- [x] Design analysis and risk identification
- [x] ATtiny1614 firmware with 3-state machine + post-water lockout
- [x] Parametric OpenSCAD frame model
- [x] BOM finalized, parts sourced
- [x] Build guide, deployment checklist, species presets documented
- [ ] Breadboard prototype validated
- [ ] First 3 units assembled
- [ ] Field deployment (private property)
- [ ] 4-week field data collected
- [ ] Public repo launch

## V1 (after MVP field data)

Target: connected devices with BLE, community deployments.

- [ ] ESP32-C3 firmware — full state machine + BLE advertising
- [ ] Night detection via solar panel voltage
- [ ] Adaptive 7-day rolling moisture baseline
- [ ] PWA — BLE scan, watering log, tree profiles
- [ ] QR code tree landing pages (static site generator)
- [ ] 3 frame sizes (small/medium/large ring sets)
- [ ] PVC pipe frame validated with build photos
- [ ] Plywood laser-cut DXF files designed
- [ ] 10 species presets with field-calibrated thresholds
- [ ] Multilingual signage (EN, ES, FR, DE, TR, AR, PT)
- [ ] Hackaday.io project page published
- [ ] First community pilot (school or municipal partner)

## V2 (community scale)

Target: municipal networks, open data platform.

- [ ] LoRaWAN variant for wide-area networks
- [ ] Custom PCB (KiCad, JLCPCB-ready)
- [ ] Biocomposite or recycled-plastic frame option
- [ ] Municipal dashboard (live map of tree status)
- [ ] Freeze / heatwave alerts
- [ ] Deer / rabbit shield accessory ring
- [ ] Open dataset published (anonymized survival + watering data)
- [ ] Teacher curriculum pack (6 lessons: biology + electronics + civics)
- [ ] Partner integrations (Trees.org, Arbor Day Foundation, Sensor.Community)

## Principles

- Do not build V1 features until MVP has survived one summer
- Field data redesigns half of V1 — expect it
- The app is sugar, not a dependency — device must always work offline
- No monetization until field viability is proven
