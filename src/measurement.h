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
#include "string.h"
#include "stdio.h"

struct Measurement {
    uint32_t timestamp;
    uint8_t device_id;
    uint8_t channel_id;
    char data[16];

    size_t toGraphite(char* buffer, size_t max_len) {
        size_t requiredLength = System.deviceID().length() + strlen(data) + 32;
        if (max_len < requiredLength) return 0;

        snprintf(buffer, max_len, "%s.%d.%d %s %lu\r\n", System.deviceID().c_str(), device_id, channel_id, data, timestamp);
        return strlen(buffer);
    }
};

#endif // IG2C_MEASUREMENT_H