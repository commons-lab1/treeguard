/**
 * TreeGuard Net — ESP32-C3 connected version (V1)
 *
 * Adds to Mini:
 *   - BLE advertising of tree state (for PWA discovery)
 *   - Solar panel voltage monitoring (night detection proxy)
 *   - Adaptive 7-day rolling baseline calibration
 *   - OTA firmware updates (stretch goal)
 *
 * State machine is identical to treeguard-mini.
 * This file is a skeleton — flesh out after MVP field validation.
 */

#include <Arduino.h>

// TODO: Implement after MVP field data informs the design
// Key additions over mini:
//
// 1. BLE beacon: advertise tree ID + state every 5 seconds
//    - Scannable by PWA for "nearby thirsty trees" feature
//    - Payload: [tree_id:4][state:1][moisture_pct:1][battery_pct:1]
//
// 2. Night detection via solar panel voltage:
//    - Solar ADC < 100 for >30 min = night → suppress LEDs
//    - More reliable than hardcoded schedule, adapts to seasons
//
// 3. Adaptive calibration:
//    - Track 7-day rolling min/max of moisture readings
//    - Adjust threshold_dry relative to observed range
//    - Handles soil type and seasonal variation automatically
//
// 4. Deep sleep with BLE wakeup:
//    - ESP32-C3 deep sleep ~5 µA
//    - Wake on RTC timer (30 min) or BLE connection (config)

void setup() {
    Serial.begin(115200);
    Serial.println("TreeGuard Net v0.1 — skeleton");
    // TODO: implement
}

void loop() {
    delay(1000);
}
