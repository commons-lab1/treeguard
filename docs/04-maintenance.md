# Maintenance Guide

## Common issues and fixes

### LED not lighting
1. Check battery voltage (>3.0V for CR123A, >3.2V for 18650)
2. Check LED solder joints
3. Run self-test: hold reset button 3 seconds, all LEDs should cycle
4. If V1 solar version: check TP4056 charge LED (red=charging, blue=full)

### Sensor reads always wet or always dry
1. Pull sensor probe, inspect for damage or corrosion
2. Check wire connections at PCB
3. Test in air (should read dry) and cup of water (should read wet)
4. If readings don't change: sensor is dead, replace it (~$1.20)
5. If readings are inverted: wires swapped at connector

### Frame damage
- Single broken ring: print or source replacement, swap out
- Spine cracked: reprint; UV-degraded PETG gets brittle after 2-3 years
- Ground stake loose: re-drive or replace; check soil compaction

### Battery replacement
- **CR123A (MVP):** twist-open enclosure, swap cell, reseal with silicone
- **18650 (V1):** disconnect JST, slide out cell, test replacement cell voltage before inserting

### Water ingress
Signs: erratic readings, LED flickering, visible condensation
1. Open enclosure
2. Dry all components with compressed air
3. Inspect gasket groove — reapply silicone if bead is cracked
4. Check cable gland seals
5. Reapply conformal coating if PCB shows corrosion

## Seasonal notes

### Summer
- Check more frequently (every 2 weeks if heatwave)
- Solar panel may need cleaning (dust, bird droppings)
- Battery should stay healthy with solar harvest

### Winter
- Trees are dormant, watering needs drop dramatically
- Firmware reduces check frequency automatically below 5C
- If hard freeze expected: 18650 cells perform poorly below -10C; consider retrieval in extreme climates
- Frame may accumulate ice — designed to handle it but inspect after thaw

### Spring
- Resume monthly checks
- Expand frame if tree has grown significantly over winter
- Re-calibrate sensor if soil composition changed (mulch added, etc.)
