#include <Arduino.h>
#include "utils.h"

void Utils::customDelay(unsigned long ms) {
    unsigned long start = millis();
    while (millis() - start < ms) {
        // Espera
    }
}