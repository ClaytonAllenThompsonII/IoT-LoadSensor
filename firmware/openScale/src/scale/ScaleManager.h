#ifndef SCALEMANAGER_H
#define SCALEMANAGER_H

#include "HX711.h"

class ScaleManager {
public:
    static void initialize();
    static void update();
    static void tare();
    static void calibrate(float calibrationFactor);
    static float getWeight();

private:
    static HX711 scale;
};

#endif