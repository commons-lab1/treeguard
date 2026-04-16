/**
 * TreeGuard Mini — ATtiny1614 offline MVP
 *
 * State machine:
 *   SLEEPING  → wake every 30 min, sample sensor
 *   THIRSTY   → red slow pulse (daytime only) when soil dry >6h
 *   HAPPY     → green breath for 2h after watering detected
 *   SICK      → amber double-blink when battery low or sensor fault
 *
 * Post-water lockout: 18h (configurable per species preset)
 */

#include <Arduino.h>
#include <avr/sleep.h>
#include "presets.h"

// --- Pin definitions (overridable via build_flags) ---
#ifndef MOISTURE_PIN
#define MOISTURE_PIN PIN_PA1
#endif
#ifndef LED_RED_PIN
#define LED_RED_PIN PIN_PA4
#endif
#ifndef LED_GREEN_PIN
#define LED_GREEN_PIN PIN_PA5
#endif
#ifndef SENSOR_POWER_PIN
#define SENSOR_POWER_PIN PIN_PA6
#endif

// --- Timing constants ---
static constexpr uint32_t SAMPLE_INTERVAL_MS     = 1800000UL; // 30 minutes
static constexpr uint32_t HAPPY_DURATION_MS       = 7200000UL; // 2 hours
static constexpr uint32_t DRY_CONFIRM_MS          = 21600000UL; // 6 hours sustained dry
static constexpr uint32_t PULSE_ON_MS             = 3000;
static constexpr uint32_t PULSE_OFF_MS            = 27000;
static constexpr uint16_t MOISTURE_JUMP_THRESHOLD = 80; // ~8% of 1023 range

// --- Night suppression (approximate, no RTC) ---
// Without RTC we can't know real time. For MVP, LED runs 24h.
// V1 with ESP32 uses NTP or solar-panel voltage drop as proxy for night.

// --- State ---
enum class State : uint8_t {
    SLEEPING,
    THIRSTY,
    HAPPY,
    SICK
};

static State state = State::SLEEPING;
static uint16_t lastMoisture = 0;
static uint32_t dryStartMs = 0;
static uint32_t happyStartMs = 0;
static uint32_t lockoutEndMs = 0;
static SpeciesPreset preset;

// --- DIP switch reading (simplified: 2 pins → 4 presets) ---
static uint8_t readPresetId() {
    // For MVP, read PA2 and PA3 as DIP inputs with internal pullups
    pinMode(PIN_PA2, INPUT_PULLUP);
    pinMode(PIN_PA3, INPUT_PULLUP);
    uint8_t id = 0;
    if (!digitalRead(PIN_PA2)) id |= 1;
    if (!digitalRead(PIN_PA3)) id |= 2;
    return id;
}

// --- Sensor ---
static uint16_t readMoisture() {
    digitalWrite(SENSOR_POWER_PIN, HIGH);
    delay(200); // sensor stabilization
    uint16_t val = analogRead(MOISTURE_PIN);
    digitalWrite(SENSOR_POWER_PIN, LOW);
    return val;
}

// --- LED patterns ---
static void ledOff() {
    digitalWrite(LED_RED_PIN, LOW);
    digitalWrite(LED_GREEN_PIN, LOW);
}

static void pulseRed() {
    digitalWrite(LED_RED_PIN, HIGH);
    delay(PULSE_ON_MS);
    digitalWrite(LED_RED_PIN, LOW);
}

static void breathGreen() {
    // Simple on/off approximation; PWM breath in V1
    digitalWrite(LED_GREEN_PIN, HIGH);
    delay(1500);
    digitalWrite(LED_GREEN_PIN, LOW);
}

static void doubleBlinkAmber() {
    // Both LEDs = amber-ish
    digitalWrite(LED_RED_PIN, HIGH);
    digitalWrite(LED_GREEN_PIN, HIGH);
    delay(150);
    ledOff();
    delay(150);
    digitalWrite(LED_RED_PIN, HIGH);
    digitalWrite(LED_GREEN_PIN, HIGH);
    delay(150);
    ledOff();
}

// --- Battery check (read VCC via internal reference) ---
static bool batteryLow() {
    // ATtiny1614: read internal 1.1V ref against VCC
    // If VCC < 2.5V, battery is nearly dead
    // Simplified: use analogReference + known divider in V1
    // For MVP with CR123A (3.0V nominal), this is a rough check
    return false; // TODO: implement with ADC internal ref
}

// --- Self-test mode ---
static void selfTest() {
    for (int i = 0; i < 3; i++) {
        digitalWrite(LED_RED_PIN, HIGH);
        delay(300);
        ledOff();
        delay(200);
    }
    for (int i = 0; i < 3; i++) {
        digitalWrite(LED_GREEN_PIN, HIGH);
        delay(300);
        ledOff();
        delay(200);
    }
    doubleBlinkAmber();
    delay(500);

    uint16_t moisture = readMoisture();
    // Blink raw value in binary (10 bits) on red LED
    for (int bit = 9; bit >= 0; bit--) {
        if (moisture & (1 << bit)) {
            digitalWrite(LED_RED_PIN, HIGH);
            delay(500);
        } else {
            digitalWrite(LED_RED_PIN, HIGH);
            delay(100);
        }
        ledOff();
        delay(300);
    }
}

// --- Main ---
void setup() {
    pinMode(LED_RED_PIN, OUTPUT);
    pinMode(LED_GREEN_PIN, OUTPUT);
    pinMode(SENSOR_POWER_PIN, OUTPUT);
    pinMode(MOISTURE_PIN, INPUT);

    ledOff();
    digitalWrite(SENSOR_POWER_PIN, LOW);

    uint8_t presetId = readPresetId();
    preset = getPreset(presetId);

    // Self-test if reset held (check if this is a fresh power-on with DIP all high)
    // Simplified: always do a quick LED flash on boot
    digitalWrite(LED_GREEN_PIN, HIGH);
    delay(500);
    ledOff();

    lastMoisture = readMoisture();
}

void loop() {
    uint32_t now = millis();

    // --- Sample sensor ---
    uint16_t moisture = readMoisture();

    // --- Detect watering event (sudden moisture jump) ---
    bool justWatered = false;
    if (moisture > lastMoisture + MOISTURE_JUMP_THRESHOLD) {
        justWatered = true;
    }

    // --- Check for sensor fault ---
    if (moisture == 0 || moisture >= 1020 || batteryLow()) {
        state = State::SICK;
    }
    // --- State transitions ---
    else if (justWatered && now > lockoutEndMs) {
        state = State::HAPPY;
        happyStartMs = now;
        lockoutEndMs = now + (uint32_t)preset.lockout_hours * 3600000UL;
    }
    else if (state == State::HAPPY) {
        if (now - happyStartMs > HAPPY_DURATION_MS) {
            state = State::SLEEPING;
        }
    }
    else if (moisture < preset.threshold_dry) {
        if (dryStartMs == 0) {
            dryStartMs = now;
        }
        if (now - dryStartMs > DRY_CONFIRM_MS && now > lockoutEndMs) {
            state = State::THIRSTY;
        }
    }
    else {
        dryStartMs = 0;
        if (state == State::THIRSTY) {
            state = State::SLEEPING;
        }
    }

    lastMoisture = moisture;

    // --- LED output ---
    switch (state) {
        case State::THIRSTY:
            pulseRed();
            delay(PULSE_OFF_MS);
            break;

        case State::HAPPY:
            breathGreen();
            delay(PULSE_OFF_MS);
            break;

        case State::SICK:
            doubleBlinkAmber();
            delay(5000);
            break;

        case State::SLEEPING:
        default:
            ledOff();
            // Deep sleep for sample interval
            // ATtiny1614 RTC-based sleep for 30 min
            // Simplified: use delay for now, proper sleep in production
            delay(SAMPLE_INTERVAL_MS);
            break;
    }
}
