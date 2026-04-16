# Hardware

## Schematics

KiCad schematics and PCB files will be added after breadboard validation.
For now, use the wiring diagrams below to build on stripboard or perfboard.

## MVP wiring (ATtiny1614)

```
                        ┌──────────────┐
            CR123A +  ──┤ VCC      PA0 ├── (unused)
            CR123A -  ──┤ GND      PA1 ├── Soil Sensor OUT (analog)
                        ├ PA7      PA2 ├── DIP Switch 1 (pullup)
      DIP Switch 3  ────┤ PA7      PA3 ├── DIP Switch 2 (pullup)
          Sensor VCC ◄──┤ PA6      PA4 ├──►220Ω──►RED LED──►GND
                        ├ PA5      PB3 ├── Self-test jumper (pullup, GND=test)
  220Ω──►GRN LED──►GND─┤ PA5      PB2 ├── (UPDI programming)
                        └──────────────┘
                          ATtiny1614

    Capacitive Soil Sensor v2.0
    ┌─────────┐
    │ VCC ────┼── PA6 (switched power, saves battery between reads)
    │ GND ────┼── GND
    │ AOUT ───┼── PA1
    └─────────┘

    DIP Switch (3-position, active-low with internal pullups)
    ┌─────────────┐
    │ SW1 ────────┼── PA2 (pullup) → bit 0
    │ SW2 ────────┼── PA3 (pullup) → bit 1
    │ SW3 ────────┼── PA7 (pullup) → bit 2
    │ COM ────────┼── GND
    └─────────────┘
    See docs/02-species-presets.md for DIP switch → preset mapping
```

## V1 wiring (ESP32-C3 Super Mini)

```
                        ┌──────────────────┐
              USB-C  ───┤ USB          3V3 ├── Sensor VCC (via GPIO6 switch)
                        ├ GND          GND ├── Common GND
    Solar+ ──►TP4056 ──┤ 5V            G0 ├── Solar ADC (voltage divider)
                        ├ G1            G1 ├── Soil Sensor OUT (analog)
                        ├ G2            G4 ├──►220Ω──►RED LED──►GND
                        ├ G3            G5 ├──►220Ω──►GRN LED──►GND
                        ├ G6            G7 ├── (unused)
          Sensor pwr ◄──┤ G6                │
                        └──────────────────┘
                         ESP32-C3 Super Mini

    TP4056 + Protection Module (HW-107)
    ┌───────────┐
    │ IN+  ─────┼── Solar panel +
    │ IN-  ─────┼── Solar panel -
    │ BAT+ ─────┼── 18650 +
    │ BAT- ─────┼── 18650 -
    │ OUT+ ─────┼── ESP32 5V
    │ OUT- ─────┼── ESP32 GND
    └───────────┘

    Solar voltage divider (for night detection):
    Solar+ ──►[10kΩ]──┬──►GPIO0 (ADC)
                       └──[10kΩ]──►GND
```

## PCB

Custom PCB files will be added after breadboard validation. Target: single-sided, hand-solderable, 30x40mm.

For now, build on stripboard or perfboard per the wiring above.

## BOM

See [bom.csv](bom.csv) for full parts list with sourcing.
