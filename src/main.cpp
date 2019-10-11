/*
 * igrill-2-cloud - Load temperature data from Weber iGrill2 products and beam
 * it to the cloud for worldwide monitoring.
 *
 * main.cpp - handle the application loop
 *
 */

#include "Particle.h"

SYSTEM_MODE(MANUAL);

#include "igrill-config.h"

#include "circular_buffer.h"

#include "output/output.h"

#include "input/dummy_input.h"
#include "input/igrill_scanner_input.h"
#include "input/input.h"

static CircularBuffer<Measurement> measurementBuffer(IG2C_MEAS_BUFFER_SIZE);
static Input* inputArray[IG2C_MAX_INPUT_DEVICES];

void setup() {
    output_setup();

    memset(inputArray, 0, IG2C_MAX_INPUT_DEVICES * sizeof(void*));
    inputArray[0] = new DummyInput(0, 5000, 1);
    inputArray[1] = new iGrillScannerInput(1, 1500, inputArray);

    Measurement::setInputArray(inputArray);
}

void loop() {
    system_tick_t loopStart = millis();

    for (size_t i=0; i<IG2C_MAX_INPUT_DEVICES; ++i) {
        if (inputArray[i] != nullptr) {
            inputArray[i]->Read(measurementBuffer);
        }
    }

    output_data(measurementBuffer);

    system_tick_t loopEnd = millis();

    if (loopStart + IG2C_TICK_TIME_MS > loopEnd) {
        delay((loopStart + IG2C_TICK_TIME_MS) - loopEnd);
    }
}