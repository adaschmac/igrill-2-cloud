/*
 * igrill-2-cloud - Load temperature data from Weber iGrill2 products and beam
 * it to the cloud for worldwide monitoring.
 *
 * output.h - basic output interface, implemented by cellular.cpp and serial.cpp
 *
 */

#ifndef IG2C_OUTPUT_H
#define IG2C_OUTPUT_H

#include "circular_buffer.h"
#include "measurement.h"

void output_setup();
void output_data(CircularBuffer<Measurement>& buffer);

#endif // IG2C_OUTPUT_H