/**
 * TreeGuard Net — ESP32-C3 connected version (V1)
 *
 * Identical state machine to treeguard-mini, plus:
 *   - BLE advertising of tree state (for PWA discovery)
 *   - Solar panel voltage monitoring (night detection proxy)
 *   - Adaptive 7-day rolling baseline calibration
 *   - OTA firmware updates (stretch goal)
 *
 * This is a compilable skeleton. Core state machine works;
 * BLE and adaptive features are stubbed for post-MVP development.
 */

#include <Arduino.h>

// --- Pin definitions (overridable via build_flags) ---
#ifndef MOISTURE_PIN
#define MOISTURE_PIN 1
#endif
#ifndef LED_RED_PIN
#define LED_RED_PIN 4
#endif
#ifndef LED_GREEN_PIN
#define LED_GREEN_PIN 5
#endif
#ifndef SENSOR_POWER_PIN
#define SENSOR_POWER_PIN 6
#endif
#ifndef SOLAR_ADC_PIN
#define SOLAR_ADC_PIN 0
#endif

// --- Timing ---
static constexpr uint32_t SAMPLE_INTERVAL_MS     = 1800000UL; // 30 min
static constexpr uint32_t HAPPY_DURATION_MS       = 7200000UL; // 2 hours
static constexpr uint32_t DRY_CONFIRM_MS          = 21600000UL; // 6 hours
static constexpr uint32_t PULSE_ON_MS             = 3000;
static constexpr uint32_t PULSE_OFF_MS            = 27000;
static constexpr uint16_t MOISTURE_JUMP_THRESHOLD = 330; // ~8% of 4095

// Night detection: solar ADC below this for 30+ min = night
static constexpr uint16_t SOLAR_NIGHT_THRESHOLD = 100;
static constexpr uint32_t NIGHT_CONFIRM_MS      = 1800000UL; // 30 min

// --- Default thresholds (overridden by BLE config or EEPROM) ---
static uint16_t thresholdDry  = 1228; // ~30% of 4095
static uint16_t thresholdWet  = 2048; // ~50% of 4095
static uint8_t  lockoutHours  = 18;

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
static bool isNight = false;
static uint32_t nightStartMs = 0;

// --- Millis overflow-safe helpers ---
static inline bool elapsed(uint32_t start, uint32_t duration) {
    return (uint32_t)(millis() - start) >= duration;
}

static inline bool timeAfter(uint32_t a, uint32_t b) {
    return (int32_t)(a - b) > 0;
}

// --- Sensor ---
static uint16_t readMoisture() {
    digitalWrite(SENSOR_POWER_PIN, HIGH);
    delay(200);
    uint16_t val = analogRead(MOISTURE_PIN);
    digitalWrite(SENSOR_POWER_PIN, LOW);
    return val;
}

// --- Night detection via solar panel voltage ---
static void updateNightState() {
    uint16_t solar = analogRead(SOLAR_ADC_PIN);
    if (solar < SOLAR_NIGHT_THRESHOLD) {
        if (nightStartMs == 0) nightStartMs = millis();
        if (elapsed(nightStartMs, NIGHT_CONFIRM_MS)) {
            isNight = true;
        }
    } else {
        nightStartMs = 0;
        isNight = false;
    }
}

// --- Battery check (ESP32 ADC on battery divider, placeholder) ---
static bool batteryLow() {
    // V1 hardware: voltage divider on battery → ADC pin
    // Implement after PCB design finalizes the divider ratio
    return false;
}

// --- LED patterns ---
static void ledOff() {
    digitalWrite(LED_RED_PIN, LOW);
    digitalWrite(LED_GREEN_PIN, LOW);
}

static void pulseRed() {
    if (isNight) return; // suppress at night
    digitalWrite(LED_RED_PIN, HIGH);
    delay(PULSE_ON_MS);
    digitalWrite(LED_RED_PIN, LOW);
}

static void breathGreen() {
    if (isNight) return;
    digitalWrite(LED_GREEN_PIN, HIGH);
    delay(1500);
    digitalWrite(LED_GREEN_PIN, LOW);
}

static void doubleBlinkAmber() {
    if (isNight) return;
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

// --- BLE (stub — implement after MVP field validation) ---
//
// Plan:
//   BLE service UUID: 0000ff01-0000-1000-8000-00805f9b34fb
//   Characteristics:
//     - tree_state (read, notify): [state:1][moisture_pct:1][battery_pct:1]
//     - tree_id (read): 4-byte unique ID
//     - config (write): [threshold_dry:2][threshold_wet:2][lockout_hours:1]
//
//   Advertise every 5 seconds in connectable mode.
//   On BLE connection: allow config writes, store to NVS (EEPROM equivalent).

// --- Setup ---
void setup() {
    pinMode(LED_RED_PIN, OUTPUT);
    pinMode(LED_GREEN_PIN, OUTPUT);
    pinMode(SENSOR_POWER_PIN, OUTPUT);
    pinMode(MOISTURE_PIN, INPUT);
    pinMode(SOLAR_ADC_PIN, INPUT);

    ledOff();
    digitalWrite(SENSOR_POWER_PIN, LOW);

    // Set 12-bit ADC resolution (ESP32 default)
    analogReadResolution(12);

    // Boot indicator
    digitalWrite(LED_GREEN_PIN, HIGH);
    delay(500);
    ledOff();

    lastMoisture = readMoisture();

    Serial.begin(115200);
    Serial.println("TreeGuard Net v0.1");
}

// --- Main loop ---
void loop() {
    uint32_t now = millis();

    uint16_t moisture = readMoisture();
    updateNightState();

    bool justWatered = (moisture > lastMoisture + MOISTURE_JUMP_THRESHOLD);
    bool sensorFault = (moisture == 0 || moisture >= 4090);
    bool lowBatt = batteryLow();

    if (sensorFault || lowBatt) {
        state = State::SICK;
    }
    else if (justWatered && timeAfter(now, lockoutEndMs)) {
        state = State::HAPPY;
        happyStartMs = now;
        lockoutEndMs = now + (uint32_t)lockoutHours * 3600000UL;
        dryStartMs = 0;
    }
    else if (state == State::HAPPY) {
        if (elapsed(happyStartMs, HAPPY_DURATION_MS)) {
            state = State::SLEEPING;
        }
    }
    else if (moisture < thresholdDry) {
        if (dryStartMs == 0) dryStartMs = now;
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
            // ESP32-C3 deep sleep: esp_sleep_enable_timer_wakeup(30*60*1000000ULL);
            // esp_deep_sleep_start();
            // For prototype, use delay:
            delay(SAMPLE_INTERVAL_MS);
            break;
    }
}
