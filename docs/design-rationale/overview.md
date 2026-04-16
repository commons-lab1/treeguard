# Design Rationale

This document explains *why* each major decision was made. If you want to change the design, read this first. If you disagree with the reasoning, open a discussion with better data.

## Why capacitive sensor, not resistive

Resistive soil sensors (two exposed metal probes) corrode via electrolysis within 4-8 weeks in wet soil. Every "smart planter" tutorial uses them because they cost $0.30. Every outdoor deployment fails because of them. Capacitive sensors cost $1 more and last years.

## Why 3 LED states, not 2

Binary "dry/wet" creates two problems:
1. No feedback after watering — person walks away unsure if they helped
2. No way for next person to know tree was just watered — leads to overwatering

The HAPPY (green) state closes the social feedback loop: "you helped, and the next person knows to move on."

## Why 18-hour post-water lockout

Water takes 2-6 hours to percolate to root depth in clay soils. The sensor may still read "dry" at probe depth even though plenty of water is on its way down. Without lockout, the LED would stay red and invite more watering. 18 hours is conservative enough for clay and doesn't under-alert for sandy soils (where percolation is fast and drying is also fast).

## Why night LED suppression

- Artificial light at night disrupts insect navigation, bird migration, and plant growth cycles
- Blinking LEDs in residential areas generate complaints and removal pressure
- Nobody is watering trees at 2 AM
- The device should be invisible when not useful

## Why PETG, not PLA

PLA glass transition is ~60C. A black PLA frame in direct summer sun can hit 70C+ and warp. PLA also becomes brittle below 0C and degrades with UV exposure within months. PETG handles -40C to 80C, resists UV much better, and is recyclable (code 1).

## Why ground stakes, never trunk attachment

Any rigid attachment to a sapling trunk will girdle and kill the tree as it grows. This is the #1 mistake in amateur tree protection. The frame must be a free-standing cage anchored to the ground.

## Why security Torx, not standard screws

Not truly tamper-proof — anyone with a $3 bit set can open it. But it raises the effort threshold above "bored kid with a pocket tool" and signals "this is intentional infrastructure, not abandoned junk."

## Why offline-first architecture

The device must function with zero network connectivity. WiFi requires infrastructure. BLE requires a nearby phone. LoRa requires gateways. None of these can be assumed for a $12 device deployed on a random sidewalk. Connectivity is valuable but optional — the red LED IS the interface.

## Why no crowdfunding or kit sales initially

Every open hardware project that monetizes before proving field viability gets trapped: you're now shipping kits instead of iterating the design. The product-market fit for TreeGuard is "does the tree survive?" and that takes a full growing season to answer.
