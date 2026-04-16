#pragma once
#include <stdint.h>

struct SpeciesPreset {
    uint16_t threshold_dry;   // ADC value (0-1023): below this = dry
    uint16_t threshold_wet;   // ADC value: above this after jump = watered
    uint8_t  lockout_hours;   // Hours to suppress alerts after watering
};

// Presets mapped to DIP switch positions (0-5 for 3-bit DIP)
// ADC values assume 10-bit (0-1023), calibrated capacitive sensor
// where 0 = dry (air) and 1023 = wet (water)
//
// See docs/02-species-presets.md for species mapping

static constexpr SpeciesPreset PRESETS[] = {
    // ID 0: Default / Unknown — conservative starting point
    { .threshold_dry = 307, .threshold_wet = 512, .lockout_hours = 18 },

    // ID 1: High water (Willow, Birch, Poplar)
    { .threshold_dry = 409, .threshold_wet = 563, .lockout_hours = 12 },

    // ID 2: Moderate water (Oak, Maple, Linden, Elm)
    { .threshold_dry = 307, .threshold_wet = 512, .lockout_hours = 18 },

    // ID 3: Low water (Pine, Olive, Juniper)
    { .threshold_dry = 204, .threshold_wet = 409, .lockout_hours = 24 },

    // ID 4: Fruit trees (Apple, Cherry, Pear)
    { .threshold_dry = 358, .threshold_wet = 563, .lockout_hours = 16 },

    // ID 5: Mediterranean (Cypress, Cork Oak, Carob)
    { .threshold_dry = 153, .threshold_wet = 358, .lockout_hours = 24 },
};

static constexpr uint8_t PRESET_COUNT = sizeof(PRESETS) / sizeof(PRESETS[0]);

inline SpeciesPreset getPreset(uint8_t id) {
    if (id >= PRESET_COUNT) id = 0;
    return PRESETS[id];
}
