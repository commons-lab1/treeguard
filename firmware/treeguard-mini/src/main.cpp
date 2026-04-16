/**
 * TreeGuard Mini — ATtiny1614 offline MVP
 *
 * State machine:
 *   SLEEPING  → wake every 30 min, sample sensor
 *   THIRSTY   → red slow pulse (daytime only) when soil dry >6h
 *   HAPPY     → green breath for 2h after watering detected
 *   SICK      → amber double-blink when battery low or sensor fault
 *
 * Post-water lockout: configurable per species preset (default 18h)
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

// DIP switch pins (3-bit = 6 presets + 2 reserved)
#define DIP_PIN_0 PIN_PA2
#define DIP_PIN_1 PIN_PA3
#define DIP_PIN_2 PIN_PA7

// Self-test trigger pin (jumper to GND on boot = enter self-test)
#define SELFTEST_PIN PIN_PB3

// --- Timing constants ---
static constexpr uint32_t SAMPLE_INTERVAL_MS     = 1800000UL; // 30 minutes
static constexpr uint32_t HAPPY_DURATION_MS       = 7200000UL; // 2 hours
static constexpr uint32_t DRY_CONFIRM_MS          = 21600000UL; // 6 hours sustained dry
static constexpr uint32_t PULSE_ON_MS             = 3000;
static constexpr uint32_t PULSE_OFF_MS            = 27000;
static constexpr uint16_t MOISTURE_JUMP_THRESHOLD = 80; // ~8% of 1023 range

// Battery threshold: CR123A nominal 3.0V, cutoff ~2.0V
// ADC reads VCC via internal 1.1V reference: ADC = 1023 * 1.1 / VCC
// VCC=2.5V → ADC ≈ 450, VCC=3.0V → ADC ≈ 375
static constexpr uint16_t BATTERY_LOW_ADC = 450; // ~2.5V

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

// --- Millis overflow-safe elapsed time helper ---
static inline bool elapsed(uint32_t start, uint32_t duration) {
    return (uint32_t)(millis() - start) >= duration;
}

// Returns true if 'a' is after 'b' (overflow-safe)
static inline bool timeAfter(uint32_t a, uint32_t b) {
    return (int32_t)(a - b) > 0;
}

// --- DIP switch reading (3 pins → 6 usable presets) ---
static uint8_t readPresetId() {
    pinMode(DIP_PIN_0, INPUT_PULLUP);
    pinMode(DIP_PIN_1, INPUT_PULLUP);
    pinMode(DIP_PIN_2, INPUT_PULLUP);
    delay(1); // settle
    uint8_t id = 0;
    if (!digitalRead(DIP_PIN_0)) id |= 1;
    if (!digitalRead(DIP_PIN_1)) id |= 2;
    if (!digitalRead(DIP_PIN_2)) id |= 4;
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

// --- Battery check via internal 1.1V reference ---
static uint16_t readVccAdc() {
    // ATtiny1614: select internal 1.1V reference, measure against VCC
    // Configure ADC to read internal 1.1V bandgap
    analogReference(INTERNAL);
    // Read from internal 1.1V channel
    // On megaTinyCore, ADC channel for internal ref varies; use analogRead
    // with the special channel if available. Simplified: read and restore.
    uint16_t raw = analogRead(ADC_INTREF);
    analogReference(VDD);
    return raw;
}

static bool batteryLow() {
    uint16_t vccAdc = readVccAdc();
    // Higher ADC value = lower VCC (inverse relationship)
    return vccAdc > BATTERY_LOW_ADC;
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

// --- Self-test mode ---
static void selfTest() {
    // Cycle red
    for (int i = 0; i < 3; i++) {
        digitalWrite(LED_RED_PIN, HIGH);
        delay(300);
        ledOff();
        delay(200);
    }
    // Cycle green
    for (int i = 0; i < 3; i++) {
        digitalWrite(LED_GREEN_PIN, HIGH);
        delay(300);
        ledOff();
        delay(200);
    }
    // Cycle amber
    doubleBlinkAmber();
    delay(500);

    // Read and display moisture as binary on red LED
    uint16_t moisture = readMoisture();
    for (int bit = 9; bit >= 0; bit--) {
        digitalWrite(LED_RED_PIN, HIGH);
        delay((moisture & (1 << bit)) ? 500 : 100);
        ledOff();
        delay(300);
    }

    // Battery indicator: green=OK, red=low
    delay(500);
    if (batteryLow()) {
        digitalWrite(LED_RED_PIN, HIGH);
    } else {
        digitalWrite(LED_GREEN_PIN, HIGH);
    }
    delay(1000);
    ledOff();
}

// --- Main ---
void setup() {
    pinMode(LED_RED_PIN, OUTPUT);
    pinMode(LED_GREEN_PIN, OUTPUT);
    pinMode(SENSOR_POWER_PIN, OUTPUT);
    pinMode(MOISTURE_PIN, INPUT);
    pinMode(SELFTEST_PIN, INPUT_PULLUP);

    ledOff();
    digitalWrite(SENSOR_POWER_PIN, LOW);

    // Check for self-test: PB3 jumpered to GND on power-up
    delay(50); // debounce
    if (!digitalRead(SELFTEST_PIN)) {
        selfTest();
    }

    uint8_t presetId = readPresetId();
    preset = getPreset(presetId);

    // Boot indicator: brief green flash
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
    bool justWatered = (moisture > lastMoisture + MOISTURE_JUMP_THRESHOLD);

    // --- Check for faults ---
    bool sensorFault = (moisture == 0 || moisture >= 1020);
    bool lowBatt = batteryLow();

    if (sensorFault || lowBatt) {
        state = State::SICK;
    }
    // --- State transitions (millis-overflow-safe) ---
    else if (justWatered && timeAfter(now, lockoutEndMs)) {
        state = State::HAPPY;
        happyStartMs = now;
        lockoutEndMs = now + (uint32_t)preset.lockout_hours * 3600000UL;
        dryStartMs = 0;
    }
    else if (state == State::HAPPY) {
        if (elapsed(happyStartMs, HAPPY_DURATION_MS)) {
            state = State::SLEEPING;
        }
    }
    else if (moisture < preset.threshold_dry) {
        if (dryStartMs == 0) {
            dryStartMs = now;
        }
        if (elapsed(dryStartMs, DRY_CONFIRM_MS) && timeAfter(now, lockoutEndMs)) {
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

    // --- LED output + sleep ---
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
            // Use power-down sleep with WDT wakeup for real deployment.
            // The ATtiny1614 RTC can wake from standby every 30 min at ~0.7µA.
            // For prototype/breadboard testing, delay() works but wastes power.
            //
            // Production sleep implementation:
            //   RTC.PITCTRLA = RTC_PERIOD_CYC32768_gc | RTC_PITEN_bm; // ~1s tick
            //   set_sleep_mode(SLEEP_MODE_PWR_DOWN);
            //   sleep_enable();
            //   // count 1800 RTC wakeups for 30 min
            //
            delay(SAMPLE_INTERVAL_MS);
            break;
    }
}
