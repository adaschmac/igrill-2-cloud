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

class Input;

class Measurement {
public:
    uint32_t timestamp;
    uint8_t device_id;
    uint8_t channel_id;
    char data[16];

    size_t toGraphite(char* buffer, size_t max_len);

    static void setInputArray(Input** array) { m_inputData = array; };

private:
    static Input** m_inputData;
};

#endif // IG2C_MEASUREMENT_H