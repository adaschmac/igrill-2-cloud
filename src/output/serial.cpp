/*
 * igrill-2-cloud - Load temperature data from Weber iGrill2 products and beam
 * it to the cloud for worldwide monitoring.
 *
 * serial.cpp - output measurement data over the USB serial connection
 * 
 */

#include "Particle.h"

#include "igrill-config.h"

#if IG2C_OUTPUT_SERIAL

#include "output/output.h"

static TCPClient clientConn;

void output_setup() {
    Serial.begin(IG2C_SERIAL_BAUD);
}

void output_data(CircularBuffer<Measurement>& buffer) {
    Measurement* meas = buffer.ReadElement();
    while (meas != nullptr) {
        String measStr = meas.toGraphite();
        Serial.write(measStr.c_str(), measStr.length());
        meas = buffer.ReadElement();
    }
}

#endif // IG2C_OUTPUT_SERIAL