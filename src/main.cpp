/*
 * igrill-2-cloud - Load temperature data from Weber iGrill2 products and beam
 * it to the cloud for worldwide monitoring.
 *
 * main.cpp - handle the application loop
 *
 */

#include "Particle.h"

#include "igrill-config.h"

#include "output/output.h"

SYSTEM_MODE(IG2C_SYSTEM_MODE)

static CircularBuffer<Measurement> measurementBuffer(20);

void setup() {
    output_setup();
}

void loop() {

}