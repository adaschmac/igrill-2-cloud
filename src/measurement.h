/*
 * igrill-2-cloud - Load temperature data from Weber iGrill2 products and beam
 * it to the cloud for worldwide monitoring.
 *
 * measurement.h - Basic storage element for measurement data
 * 
 */

#ifndef IG2C_MEASUREMENT_H
#define IG2C_MEASUREMENT_H

#include "Particle.h"

struct Measurement {
    uint32_t timestamp;
    uint8_t device_id;
    uint8_t channel_id;
    char data[16];

    String toGraphite() {
        return System.deviceID() + "." + String(device_id) + String(channel_id) + data;
    }
};

#endif // IG2C_MEASUREMENT_H