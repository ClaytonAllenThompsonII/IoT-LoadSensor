#include "ScaleManager.h"

#define LOADCELL_DOUT_PIN 3
#define LOADCELL_SCK_PIN 2

HX711 ScaleManager::scale;

void ScaleManager::initialize() {
    scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
    scale.set_scale();
    scale.tare();
}

void ScaleManager::update() {
    // This function could handle automatic periodic updates if necessary
}

void ScaleManager::tare() {
    scale.tare();
}

// set the SCALE value; this value is used to convert the raw data to "human readable" data (measure units)
void ScaleManager::calibrate(float calibrationFactor) {
    scale.set_scale(calibrationFactor);
}

// returns get_value() divided by SCALE, that is the raw value divided by a value obtained via calibration
// times = how many readings to do = 10
float ScaleManager::getWeight() {
    return scale.get_units(10); // Average over 10 readings
}
