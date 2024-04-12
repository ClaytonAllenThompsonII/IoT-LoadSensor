#include "ScaleManager.h"

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

void ScaleManager::calibrate(float calibrationFactor) {
    scale.set_scale(calibrationFactor);
}

float ScaleManager::getWeight() {
    return scale.get_units(10); // Average over 10 readings
}
